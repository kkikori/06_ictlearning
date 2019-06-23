#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <cstdlib>
#include<thread>

using namespace std;


void show_all_digit(int num){
	int digit[3];
	extract_3digit(num,digit);
	cout << digit[2] << digit[1] << digit[0];
}

class Numeron{
	int NUM_LEN;
	int CAND_NUMS;
	int *cand_list;
	int cand_num;
public:
	int guessing_count;
	Numeron (int digit):NUM_LEN(digit),CAND_NUMS(pow(10,digit)){init_candlist();}
	void init_candlist();
	int random_choice_num();
	void count_cand_num(){
		cand_num=0;
		for(int i=0;i<CAND_NUMS;i++){
			if (cand_list[i]>0) cand_num++;
		}
	}
	void extract_digit(int num,int* places){
		places[0]=num%10;
		if(NUM_LEN==1) return ;
		places[1]= (num/10)%10;
		if(NUM_LEN==2) return;
		places[2] = num/100;
		if(NUM_LEN==3) return;
		places[4]=num/1000;
		return;
	}
	void cand_update_from_eatbite(int eat, int bite,int guessing){
		int i,g_eat,g_bite;
		for(i=0;i<CAND_NUMS;i++){
			if(cand_list[i]==0) continue;
			judge_eatbite(guessing,i,g_eat,g_bite);
			
			if(g_eat!=eat || g_bite!=bite) cand_list[i]=0;
		}
		cand_list[guessing]=0;
		guessing_count+=1;
		count_cand_num();
	}
	
	void judge_eatbite(int target,int juding, int &g_eat,int &g_bite){
		int target_l[NUM_LEN], juding_l[NUM_LEN];
		int eat=0,bite=0,i,j;
		
		extract_3digit(target,target_l);
		extract_3digit(juding,juding_l);
			
		//count eat
		for(i=0;i<NUM_LEN;i++){
			if (target_l[i]==juding_l[i]) eat++;
		}
		//count bite
		for(i=0;i<NUM_LEN;i++){
			for(j=0;j<NUM_LEN;j++){
				if (j==i) continue;
				if(target_l[i]==juding_l[j]) bite++;
			}
		}
		g_eat=eat; g_bite=bite;
	}
	void show_cand_list(){
		cout << "show cand list "<<endl;
		for(int i=0;i<CAND_NUMS;i++){
			if (cand_list[i]>0) show_all_digit(i),cout <<" ";
		}
		cout << endl;
	}
	void finish_game(int seikai){
		cout << " ================================="<<endl;
		cout << "                   ���s�� : " << guessing_count << endl;
		//cout << " �I������ɂ� Enter-key �������Ă��������B\n" << endl;
		//while(1) {if(cin.get()=='\n') exit(0);}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	int error_user_input(int num){
		int num_l[NUM_LEN];
		if(0>num || num >987) {
			cout << "���͂��ꂽ�����͖����ł�" << endl;
			return -1;
		}
		extract_3digit(num,num_l);
		if(num_l[0] == num_l[1] || num_l[0] == num_l[1] ||num_l[0] == num_l[1]){
			cout << "���͂��ꂽ���l�͓������������g���Ă��܂�" << endl;
			return -1;
		}
		return 1;
	}
};

void Numeron::init_candlist(){
	guessing_count=1;
	cand_list = new int[CAND_NUMS];
	int i,digit[NUM_LEN];
	if(NUM_LEN!=3) {
		cout << "ERROR initialize" << endl;
		exit(1);
	}
	for (i=0;i<CAND_NUMS;i++){
		extract_3digit(i,digit);
		if(digit[0]==digit[1] || digit[0]==digit[2] || digit[1]==digit[2]){
			cand_list[i]=0;
		}
		else cand_list[i]=1;
	}
	count_cand_num();
}

int Numeron::random_choice_num(){
	int i, random_n,k=0;
	//random
	srand(time(NULL));
	rand(); rand(); rand(); rand(); rand();
	random_n=rand() % (cand_num) +1;
	
	for(i=0;i<CAND_NUMS;i++){
		if (cand_list[i]==0) continue;
		k++;
		if (k==random_n){ return i;}
	}
	return -1;
}


void game_taisen(){
	//guessing
	int i, eat=0, bite=0,guess_num=0,g_n_l[3];
	Numeron numeron3(3);
	//user playing
	int pc_numeron_num = numeron3.random_choice_num();
	int user_guessing;
	
	while(1){
		cout <<endl<< " [ �Q�[���F" << numeron3.guessing_count << "��� ]";
		cout<<endl<<" ------- Your turn -----------"<<endl;
		while(1){
			cout<<" PC�̒l��\�����Ă�������->";
			cin>>user_guessing;
			if (numeron3.error_user_input(user_guessing)!=-1) break;
		}
		numeron3.judge_eatbite(user_guessing, pc_numeron_num, eat,bite);
		if (eat==3){
			cout <<endl<< "    !!!!!! YOU WIN !!!!!!!!" <<endl;
			break;
		}
		cout << " ���Ȃ��̓��͂������l��....  "<<eat<<" eat   "<< bite<< " bite  !!" << endl; 
		
		
		cout <<endl<< " ------- PC's turn -----------" <<endl;
		guess_num = numeron3.random_choice_num();
		if(guess_num<0) cout<<"EROOR1",exit(1);
		extract_3digit(guess_num,g_n_l);
		cout << " ���Ȃ��̍l�������l�� ";show_all_digit(guess_num); cout << " �ł���?" << endl;
		
		while(1){
			cout << "  input eat -> " , cin >> eat;
			cout << "  input bite -> " , cin >> bite;
			if (eat+bite >3 || eat==2 && bite==1){
				cout <<"your input is wrong" << endl <<"please input again"<<endl;
			}
			else break;
		}
		if(eat==3) {
			cout <<endl<< "    !!!!!! PC WIN !!!!!!!!" <<endl;
			break;
		}
		
		numeron3.cand_update_from_eatbite(eat,bite,guess_num);
		//numeron3.show_cand_list();
	}
	numeron3.finish_game(guess_num);
	
}

void test(){
	Numeron numeron3(3);
	int g_eat,g_bite;
	numeron3.judge_eatbite(594,15, g_eat,g_bite);
	cout << g_eat << g_bite << endl;
}


int main(){
	string rule="";
	cout <<"=========================================================="<<endl<<"                  [START NUMERON]"<<endl;
	cout <<"=========================================================="<<endl << endl;
	cout << " ���[���͒m���Ă��܂����H(y/n) -> ";
	cin >> rule;
	if (rule == "n"){
		cout << "  �Q�[���͂��Ȃ���PC�̑ΐ�ōs���܂��B" <<endl;
		cout << "  �܂��A���ꂼ��̌��̐������قȂ�3���̐������v�������ׂĂ��������B" <<endl;
		cout << "   ��)'123'��'560'�Ȃǂ�OK�A'223'��'878'�Ȃǂ�NG"<<endl;
		cout << "  PC�ƃv���[���[���m�����݂��̐����𓖂Ă������܂��B" <<endl;
		cout << "  ���Ȃ��񐔂œ��Ă�ꂽ�ق��������ł��I" <<endl<<endl;
		cout << "  PC�����Ȃ��̐����𓖂Ăɗ��܂��̂ŁA�����ꂽ�����ɑ΂���" <<endl;
		cout << "    eat�@ �̐� : ���������������ʒu�ɂ���" <<endl;
		cout << "    bite�@�̐� : �����������ʂ̈ʒu�ɂ���" <<endl;
		cout << "  �̏���Ԃ��Ă��������B" <<endl;
		cout << "    ��)���Ȃ���123���v�������ׂĂ�����... " << endl;
		cout << "      120 : 2eat 0bite" << endl;
		cout << "      691 : 0eat 1bite" << endl << endl;
	}
	cout <<"=========================================================="<<endl;
	cout <<"   Let's start game!!"<<endl;
	
	
	game_taisen();
	//test();
	return 0;
}
