const char* TO[]={"a’J","‘γ―R","’†–Ϊ•","—S“V›","w|‘εw","“s—§‘εw","©—R‚ª‹u","“c‰€’²•z","‘½–€μ","VΫq","•‘ ¬™","³Z‹g","“ϊ‹g","j“‡","‘ε‘qR","‹e–Ό","–­@›","”’y","“”’y","”½’¬","‰΅•l", 0};
const char* YO[]={"“‹","V‹΄","•iμ","Ό‘εδ","•‘ ¬™","Vμθ","‰΅•l", 0};
const char* NA[]={"μθ","Kθ","–ξό","­“‡“c","•½Τ","ό‰Ν΄","•‘ ¬™","•‘ ’†΄","•‘ V―","•‘ aƒmϋ","’Γ“cR","‹v’n","h‰Ν΄","“oΛ","’†–μ“‡","ξ“c’η","–ξ–μϋ","ξι’·ΐ","“μ‘½–€","•{’†–{’¬","•ª”{‰Ν΄","Ό•{","’J•Ϋ","–ξμ","Ό‘—§","—§μ", 0};
const char* YA[]={"•iμ","‘εθ","ά”½“c","–Ϊ•","b”δυ","a’J","΄h","‘γX–Ψ","Vh","V‘ε‹v•Ϋ","‚“c”nκ","–Ϊ”’","’r‘ά","‘ε’Λ","‘ƒ›","‹ξ","“c”¨","Ό“ϊ•ι—Ά","“ϊ•ι—Ά","ις’J","γ–μ","δ“k’¬","H—t΄","_“c","“‹","—Ly’¬","V‹΄","“c’¬","•iμ", 0};
const char* CH[]={"“‹","_“c","δ’ƒƒm…","lƒc’J","Vh","’†–μ","‚‰~›","Ά²ƒ–’J","‰¬E","Ό‰¬E","‹gΛ›","O‘ι","•‘ ‹«","“¬‹ΰδ","•‘ ¬‹ΰδ","‘•ª›","Ό‘•ª›","‘—§","—§μ", 0};
const char* HI[]={"’†–Ϊ•","b”δυ","L”φ","Z–{–Ψ","_’J’¬","‰ΰƒ–Φ","“ϊ”δ’J","‹βΐ","“‹βΐ","’z’n","”ª’–x","κ’¬","l`’¬","¬“`”n’¬","H—t΄", 0};
const char** LINE[]={TO, YO, NA, YA, CH, HI, 0};

#include<iostream>
#include<vector>
using namespace std;
vector<string> route, from, to;
void ride(string x, string y) {
	if (x==y) {
		cout<<'['<<route.size()+1<<']';
		for (string s:route) cout<<s<<"->";
		cout<<x<<endl;
	} else {
		route.push_back(x);
		for (size_t i=0; i<from.size(); i++) if (x==from[i]) {
			size_t j=0, n=route.size();
			for (j=0; j<n; j++) if (route[j]==to[i]) break;
			if (j==n) ride(to[i], y);
		}
		route.pop_back();
	}
}
int main(int argc, char* argv[]) {
	for (const char*** p=LINE; *p; p++) for (const char** q=*p; q[1]; q++) {
		size_t i, n=from.size();
		for (i=0; i<n; i++) if (from[i]==q[0] && to[i]==q[1]) break;
		if (i==n) from.push_back(q[0]), to.push_back(q[1]);
		for (i=0; i<n; i++) if (from[i]==q[1] && to[i]==q[0]) break;
		if (i==n) from.push_back(q[1]), to.push_back(q[0]);
	}
	//for (size_t i=0, n=from.size(); i<n; i++) cout<<from[i]<<"->"<<to[i]<<" ";
	if (argc!=3) return cout<<argv[0]<<" from to"<<endl, 1;
	ride(argv[1], argv[2]);
}
