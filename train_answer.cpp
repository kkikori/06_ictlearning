const char* TO[]={"�a�J","�㊯�R","���ڍ�","�S�V��","�w�|��w","�s����w","���R���u","�c�����z","������","�V�ێq","��������","���Z�g","���g","�j��","��q�R","�e��","���@��","���y","�����y","����","���l", 0};
const char* YO[]={"����","�V��","�i��","�����","��������","�V���","���l", 0};
const char* NA[]={"���","�K��","���","�����c","����","���͌�","��������","��������","�����V��","�����a�m��","�Óc�R","�v�n","�h�͌�","�o��","���쓇","��c��","����","��钷��","�쑽��","�{���{��","���{�͌�","���{","�J��","���","������","����", 0};
const char* YA[]={"�i��","���","�ܔ��c","�ڍ�","�b���","�a�J","���h","��X��","�V�h","�V��v��","���c�n��","�ڔ�","�r��","���","����","�","�c��","�����闢","���闢","��J","���","��k��","�H�t��","�_�c","����","�L�y��","�V��","�c��","�i��", 0};
const char* CH[]={"����","�_�c","�䒃�m��","�l�c�J","�V�h","����","���~��","�������J","���E","�����E","�g�ˎ�","�O��","������","��������","����������","������","��������","����","����", 0};
const char* HI[]={"���ڍ�","�b���","�L��","�Z�{��","�_�J��","������","����J","���","�����","�z�n","�����x","���꒬","�l�`��","���`�n��","�H�t��", 0};
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
