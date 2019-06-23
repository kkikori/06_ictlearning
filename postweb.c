#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include<mbstring.h>
#include<stdio.h>

typedef struct {
	void* Value;
	size_t Size, Capacity;
} MEM;
void MEMNew(MEM* mem, size_t Capacity) {
	mem->Value=malloc(Capacity), mem->Size=0, mem->Capacity=Capacity;
}
void MEMDelete(MEM* mem) {
	free(mem->Value), mem->Value=NULL, mem->Size=mem->Capacity=0;
}
void MEMClear(MEM* mem) { mem->Size=0; }
void MEMAdd(MEM* mem, void* Value, size_t size) {
	if (mem->Capacity<mem->Size+size) {
		while (mem->Capacity<mem->Size+size) mem->Capacity*=2;
		mem->Value=realloc(mem->Value, mem->Capacity);
	}
	memcpy(mem->Value+mem->Size, Value, size), mem->Size+=size;
}
void MEMCat(MEM* mem, char* str) { MEMAdd(mem, str, strlen(str)); }
void MEMDump(MEM* mem) {
	printf("MEM{Value:%p,Size:%d,Capacity:%d}\n", mem->Value, mem->Size, mem->Capacity);
}

typedef struct {
	char *Key, *Value;
} DICT;
void DICTNew(MEM* mem) { MEMNew(mem, sizeof(DICT)*100); }
void DICTDelete(MEM* mem) { MEMDelete(mem); }
void DICTClear(MEM* mem) { MEMClear(mem); }
void DICTAdd(MEM* mem, char* key, char* value) {
	DICT d={key, value};
	MEMAdd(mem, &d, sizeof(d));
}
char* DICTGet(MEM* mem, char* key) {
	DICT* d=(DICT*)mem->Value;
	for (size_t i=0, n=mem->Size/sizeof(DICT); i<n; i++) if (strcmp(d[i].Key, key)==0) return d[i].Value;
	return NULL;
}
void DICTDump(MEM* mem) {
	DICT* d=(DICT*)mem->Value;
	printf("DICT{");
	for (size_t i=0, n=mem->Size/sizeof(DICT); i<n; i++) printf("%s:%s%s", d[i].Key, d[i].Value, i<n-1?",":"");
	printf("}\n");
}

typedef struct {
	char ZipCode[8], *Address;
} POST;
MEM MemPost;
POST *Post;
size_t NPost=0;
void POSTNew() {
	MEMNew(&MemPost, sizeof(POST)*10000);
	FILE* fp=fopen("ken_all.csv", "r");
	char s[1000], d[]=",\"", *zip, *adr[3], *dup;
	size_t len[4], cat=0;
	POST post={"",NULL};
	while (fgets(s, sizeof(s), fp)) {
		strtok(s, d), strtok(NULL, d), zip=strtok(NULL, d);
		strtok(NULL, d), strtok(NULL, d), strtok(NULL, d);
		adr[0]=strtok(NULL, d), adr[1]=strtok(NULL, d), adr[2]=strtok(NULL, d);
		strtok(NULL, d), strtok(NULL, d), strtok(NULL, d), dup=strtok(NULL, d);
		if (!zip || strlen(zip)!=7 || !adr[0] || !adr[1] || !adr[2] || !dup) continue;
		len[0]=strlen(adr[0]), len[1]=strlen(adr[1]), len[2]=strlen(adr[2]);
		if (strcmp(post.ZipCode, zip)!=0 || strcmp(dup, "1")==0) {
			//if (cat>0) printf("%s %s\n", post.ZipCode, post.Address), cat=0;
			MEMAdd(&MemPost, &post, sizeof(POST));
			strcpy(post.ZipCode, zip);
			post.Address=malloc(len[0]+len[1]+len[2]+1);
			strcpy(post.Address, adr[0]);
			strcpy(post.Address+len[0], adr[1]);
			strcpy(post.Address+len[0]+len[1], adr[2]);
		} else {
			len[3]=strlen(post.Address);
			post.Address=realloc(post.Address, len[3]+len[2]+1);
			strcpy(post.Address+len[3], adr[2]);
			cat++;
		}
	}
	if (post.Address) MEMAdd(&MemPost, &post, sizeof(POST));
	fclose(fp);
	Post=MemPost.Value, NPost=MemPost.Size/sizeof(POST);
	MEMDump(&MemPost);
}
void POSTDelete() {
	for (size_t i=0; i<NPost; i++) free(Post[i].Address);
	MEMDelete(&MemPost), Post=NULL, NPost=0;
}

void Respond(MEM* dict, MEM* out) {
	char *query=DICTGet(dict, "query"), num[100];
	MEMCat(out, "<!doctype html><html><head><meta charset='shift_jis'><title>post</title></head><body>\r\n");
	MEMCat(out, "<h1>郵便番号検索サービス</h1><h2>検索する郵便番号または住所を入力</h2>");
	MEMCat(out, "<form><input type='text' name='query' value='");
	MEMCat(out, query?query:"");
	MEMCat(out, "'><input type='submit' Value='検索'></form>");
	if (query) {
		MEM keyword;
		MEMNew(&keyword, sizeof(char*)*100);
		for (char d[]="+ 　", *t=_mbstok(query, d); t; t=_mbstok(NULL, d)) MEMAdd(&keyword, &t, sizeof(char*));
		MEMDump(&keyword);
		char** kw=keyword.Value;
		
		size_t i, j, n=0, nkw=keyword.Size/sizeof(char*);
		for (i=0; i<NPost; i++) {
			for (j=0; j<nkw; j++) if (!_mbsstr(Post[i].ZipCode, kw[j]) && !_mbsstr(Post[i].Address, kw[j])) break;
			if (j==nkw) n++;
		}
		printf("found(%d)\n", n);
		MEMCat(out, "<h2>");
		for (i=0; i<nkw; i++) MEMCat(out, "「"), MEMCat(out, kw[i]), MEMCat(out, "」");
		MEMCat(out, "の検索結果（");
		itoa(n, num, 10), MEMCat(out, num);
		MEMCat(out, "件）</h2>");

		if (n>0) {
			MEMCat(out, "<table border=1><tr><th>〒</th><th>住所</th></tr>");
			for (i=0; i<NPost; i++) {
				for (j=0; j<nkw; j++) if (!_mbsstr(Post[i].ZipCode, kw[j]) && !_mbsstr(Post[i].Address, kw[j])) break;
				if (j==nkw) {
					MEMCat(out, "<tr><td>");
					MEMCat(out, Post[i].ZipCode);
					MEMCat(out, "</td><td>");
					MEMCat(out, Post[i].Address);
					MEMCat(out, "</td></tr>");
				}
			}
			MEMCat(out, "</table>");
		}
		MEMDelete(&keyword);
	}
	MEMCat(out, "\r\n</body></html>\r\n");
}

const char* Serve() {
	WSADATA wsaData;
	if (0!=WSAStartup(MAKEWORD(2,2), &wsaData)) return "WSAStartup";

	const char* port="8000";
	struct addrinfo ai_hints, *ai=NULL;
	ZeroMemory(&ai_hints, sizeof(ai_hints));
	ai_hints.ai_family=AF_INET;
	ai_hints.ai_socktype=SOCK_STREAM;
	ai_hints.ai_protocol=IPPROTO_TCP;
	ai_hints.ai_flags=AI_PASSIVE;
	if (0!=getaddrinfo(NULL, port, &ai_hints, &ai))
		return "getaddrinfo";

	SOCKET ssock=INVALID_SOCKET, csock=INVALID_SOCKET;
	if (INVALID_SOCKET==(ssock=socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol))) return "socket";
	if (SOCKET_ERROR==bind(ssock, ai->ai_addr, (int)ai->ai_addrlen)) return "bind";
	if (SOCKET_ERROR==listen(ssock, SOMAXCONN)) return "listen";
	printf("[serving http://localhost:%s]\n", port);

	char in[100000];
	MEM out, dict;
	MEMNew(&out, 100000);
	DICTNew(&dict);
	for (;;) {
		printf("[accept]"), fflush(stdout);
		if (INVALID_SOCKET==(csock=accept(ssock, NULL, NULL))) return "accept";

		printf("[recv]\n"), fflush(stdout);
		size_t len=recv(csock, in, sizeof(in)-1, 0);
		if (len<0 || len==sizeof(in)-1) goto failed;
		in[len]='\0';
		printf("%s\n", in);

		int Hex2Dec(char hex) {
			return '0'<=hex&&hex<='9'?hex-'0':
				'A'<=hex&&hex<='F'?hex-'A'+10:
				'a'<=hex&&hex<='f'?hex-'a'+10:-1;
		}
		DICTClear(&dict);
		char *p, d[]="/?=&", *key, *value;
		if ((p=strtok(in, "\r\n")) && (strtok(p, " "), p=strtok(NULL, " "))) {
			for (; (key=strtok(p, d)) && (value=strtok(NULL, d)); p=NULL) {
				char *v, *w;
				for (v=w=value; *v; v++, w++) 
					if (*v=='%') *w=Hex2Dec(v[1])*16+Hex2Dec(v[2]), v+=2; else *w=*v;
				*w='\0';
				DICTAdd(&dict, key, value);
			}
		}
		DICTDump(&dict);

		MEMClear(&out);
		MEMCat(&out, "HTTP/1.1 200 OK\r\n\r\n");
		Respond(&dict, &out);
		MEMDump(&out);
		printf("[send]"), fflush(stdout);
		if (SOCKET_ERROR==send(csock, out.Value, out.Size, 0)) goto failed;

		goto close;
		failed:
		printf("failed");
		close:
		printf("[close]\n"), fflush(stdout);
		if (SOCKET_ERROR==closesocket(csock)) printf("closesocket failed.\n");
		csock=INVALID_SOCKET;
	}
	MEMDelete(&out);

	if (csock!=INVALID_SOCKET) closesocket(csock);
	if (ssock!=INVALID_SOCKET) closesocket(ssock);
	if (ai!=NULL) freeaddrinfo(ai);
	WSACleanup();
	return "none";
}

int main() {
	POSTNew(), printf("error:%s\n", Serve()), POSTDelete();
	return 0;
}
