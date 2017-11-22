#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATION 10                  /* �ő�̉w�̐� */
#define ERROR    NULL

typedef struct Station {
	char name[20+1];                /* �w���͍ő�S�p10�����܂� */
	int  time;                      /* �O�̉w����̏��v���� */
	struct Station *next_addr;      /* ���̃f�[�^�̃A�h���X */
} STATION;

int station_num = 0;                    /* �t�@�C������ǂݍ��񂾉w�̐� */
STATION station[MAX_STATION];           /* ���l�̓��͗p�z�� */
STATION head;

int get_num(void) {
	int  i;
	int  d = 0;
	int  c = 0;
	rewind(stdin);
	for( i = 0; i < 5 && c != '\n' ; i++ ) { /* 5�P�^�܂� */
		c = getchar();
		if ( c == '\n' ) break;
		d = d * 10 + c - '0';
	}
	rewind(stdin);
	return d;
}

/* �f�[�^�t�@�C���̓ǂݍ��� */
int read_data(void)
{
	FILE *fp;
	int i;
	/* �L�����N�^�f�[�^�ǂݍ��� */
	fp =fopen("station_data.txt","r");// station_data.txt�̓ǂݍ���
	if ( fp == NULL ){
		fprintf(stderr,"CANNOT OPEN station_data.txt\n");
		return -1;
	}
	for( i = 0; i < MAX_STATION; i++, station_num++ ) {
		if ( fscanf(fp,"%s\t%d",station[i].name,&station[i].time) != 2 ) break;
		/* �܂��͓ǂݍ��񂾏��Ƀ��X�g���Ȃ� */
		station[i].next_addr = &station[i+1];
	}
	fclose(fp);
	/* ���X�g�̐擪��head.next_addr�ɃZ�b�g */
	head.next_addr = &station[0];
	/* ���X�g�̍Ō��NULL�����Ă��� */
	station[station_num-1].next_addr = NULL;
	return 0;
}

/* �H���f�[�^��\�� */
void station_disp(void)
{
	STATION *current_addr;
	current_addr = head.next_addr;
	
	while(1){
		printf("|ADDR:%20d|\n",current_addr);
		printf("|NAME:%20s|\n",current_addr->name);
		printf("|TIME:%20d|\n",current_addr->time);
		printf("|NEXT:%20d|\n",current_addr->next_addr);
		printf("\n");
		if(current_addr->next_addr == NULL)
			break;
		current_addr=current_addr->next_addr;
	}
}

/* ���͂��ꂽ�w�����X�g�ɑ��݂��Ă��邩�ǂ����̃`�F�b�N */
STATION *check(char target[])
{
	/* ���X�g�����ǂ��Ďw�肳�ꂽ�w���܂܂�Ă��邩�ǂ����m�F */
	STATION *current_addr;
	current_addr = head.next_addr;
	while(1){
		if( strcmp(target,current_addr->name)==0 )
			return current_addr;
		if(current_addr->next_addr == NULL)
			break;        
		current_addr=current_addr->next_addr;
		printf("check");
	}
	return ERROR;
}

/* �f�[�^�̒ǉ� */
void add(void)
{
	STATION *current_addr/*,*new_addr=(STATION*)malloc(sizeof(STATION))*/,*tmp;
	current_addr=head.next_addr;
	char targetChar[16];    // �ǉ�����w�̒��O�̉w�����i�[
	int a_point; /* �z��Ƃ��Ă̒ǉ��ʒu */
	int i,cmp;
	char buf[20+1];
	a_point = station_num++;
	printf("NAME =");
	fgets(station[a_point].name,16,stdin);
	for( i=0; station[a_point].name[i] != '\n' && i != 15; i++ );
	station[a_point].name[i] = '\0';
	/* �������s�R�[�h���폜���邽�߂̏��� */
	printf("TIME =");
	station[a_point].time = get_num();
	station_disp();
	do{
		printf("�ǂ̉w�̌�ɒǉ����܂���?\nNAME =");
		fgets(targetChar,16,stdin);
		for( i=0; targetChar[i] != '\n' && i != 15; i++ );
		targetChar[i] = '\0';
		/* �����s�R�[�h���폜���邽�߂̏��� */
		printf("target %s\n",targetChar);
	} while( (current_addr=check(targetChar)) == ERROR );
	/* �f�[�^�̒ǉ��i���X�g���Ȃ��j */
	printf("current %s target %s %s\n",targetChar,current_addr->name,buf);
	printf("get_num() %d",station[a_point].time);
	station[a_point].next_addr=current_addr->next_addr;
	current_addr->next_addr=&station[a_point];
	station_disp();
}

/* �f�[�^�̍폜 */
void del(void)
{
	int i;
	STATION *forDel; // �폜�������v�f���i�[����|�C���^
	STATION *current_addr,*before_addr;
	char targetChar[16];
	
	station_disp();
	do{
		printf("�ǂ̉w���폜���܂���?\nNAME =");
		fgets(targetChar,16,stdin);
		for( i=0; targetChar[i] != '\n' && i != 15; i++ );
		targetChar[i] = '\0';
		/* �����s�R�[�h���폜���邽�߂̏��� */
	} while( check(targetChar) == ERROR );

	before_addr=&head;
	current_addr = head.next_addr;

	if (current_addr->next_addr == NULL ){
		printf("station is not enough\n");
		station_disp();
		return ;
	}

	while(1){
		if(!strcmp(current_addr->name,targetChar)){
			if( current_addr->next_addr == NULL){
				before_addr->next_addr=NULL;
				break;
			}else{
				before_addr->next_addr=current_addr->next_addr;
				break;
			}
		}
		before_addr=current_addr;
		current_addr=current_addr->next_addr;
	}
	station_disp();
}

/* ���v���Ԃ̌v�Z */
void calc(void)
{
	int sum=0, i;
	
	STATION *current_addr,*from,*to;
	char targetFrom[16],targetTo[16];
	
	printf("�ǂ�����ǂ��܂ł̏��v���Ԃ��v�Z���܂����H\n");
	do{
		printf("FROM(Station Name)=");
		fgets(targetFrom,16,stdin);
		for( i=0; targetFrom[i] != '\n' && i != 15; i++ );
		targetFrom[i] = '\0';
		/* �����s�R�[�h���폜���邽�߂̏��� */
	} while( (from=check(targetFrom)) == ERROR );
	do{
		printf("TO(Station Name)=");
		fgets(targetTo,16,stdin);
		for( i=0; targetTo[i] != '\n' && i != 15; i++ );
		targetTo[i] = '\0';
		/* �����s�R�[�h���폜���邽�߂̏��� */
	} while( (to=check(targetTo)) == ERROR );
	printf("%s����",targetFrom);
	printf("%s�܂łł���\n",targetTo);
	current_addr=from->next_addr;
	//while(current_addr!=to){
	while(1){
		sum+=current_addr->time;
//		printf("sum %d %s\n",sum,current_addr->name);
		if(current_addr==to) break;
		current_addr=current_addr->next_addr;
	}//
	printf("���v���Ԃ�%d�ł�\n",sum);
}

/* ���j���[ */
int menu(void)
{
	int c;
	while( 1 ){
//		printf("1.�\\��\n");   // �\�Ǝ��̊Ԃ�"\"�͂��܂��Ȃ��i�O���Ƃǂ��Ȃ邩�A�����Ă݂Ă��悢
		printf("1.�\\��\n");   // �\�Ǝ��̊Ԃ�"\"�͂��܂��Ȃ��i�O���Ƃǂ��Ȃ邩
		printf("2.�ǉ�\n");
		printf("3.�폜\n");
		printf("4.���v����\n");
		printf("5.�I��\n");
		c = get_num();
		if( c >= 1 && c <= 5 )
			break;
		else
			printf("1?5�܂ł̐�������͂��Ă�������\n");
	}
	switch( c ){
		case 1:
			station_disp();
			break;
		case 2:
			add();
			break;
		case 3:
			del();
			break;
		case 4:
			calc();
			break;
		case 5:
			return 1;
	}
	return 0;
}

int main(void)
{
	int end = 0;
	end = read_data();
	while( !end ){
		end = menu();
	}
	return 0;
}