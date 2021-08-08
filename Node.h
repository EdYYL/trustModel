//�����
#include<iostream>
#include<list>
#include<set>
#include<queue>
#include<unordered_map> 
#include<Windows.h>
using namespace std;
#include "Packet.h"
const int THING_RES = 100;		//�������Դ����
const int EDGE_RES = 1000;		//��Ե��������Դ����
const int DATA_TYPE = 100;		//��������
const int DATA_RANGE = 50;			//���ݷ�Χ
const double INIT_DATA_TRUST = 0;
const int NoDATA = DATA_RANGE+1;	//��Ե�����������ݵı��
//const int TIMELIMIT = 1000;		//����ʱ�ޣ�
//������ͣ�����Ϊ��Ե������
const int COOPERATIVE_EDGE = -1;
const int MALICIOUS_EDGE = -2;
const int COOPERATIVE_THING = 1;
const int MALICIOUS_THING = 2;
constexpr auto REFUSEFORWARD = -2;
const double F = 0.5;			//�Ҷ���㲻ת����
const int DATA_NOT_FOUND = -1;	//δ�ҵ�����

const double SAT_DELAY;
const double SAT_BANDWIDTH;
const double WD;
const double WI;

class Node {
private:
	int x;
	int y;
	int no;						//�����
	int resource;				//�����Դ��
	int neighbourNum;			//�ھ���
	unordered_map<int, RIPRow> ripTable;//·�ɱ�
	vector<int> crossLayerAssociation;	//��������������¼�����������������¼�䷶Χ�ڵ����
	set<int> pHistory;			//�洢�չ��İ�
	map<int, double> data;
	vector<double> dataTrust;	//�������Σ��յ�����������ʵֵ���첻�����¼һ�����Σ�
	queue<Packet> sendBuffer;	//����������
	queue<Packet> receiveBuffer;//�հ�������
	vector<vector<double> > dataBuffer;//��Ե�����������ݻ�����
	
	int type;					//���࣬2��ʾ�Ʋ�,1��ʾ��Ե������,0��ʾ�����

	map<int,double> Ts //��������
	map<int,double> Tc //��������
	map<int,double> Th //��������
	
	map<int,int> num_s; //��������������Ĵ���
	map<int,int> num_us; //���������в�����Ĵ���
	map<int,int> num_c; //���������е��������
	map<int,int> num_nc; //���������еĲ��������
	map<int,int> num_fw; //Э�������е�ת������
	map<int,int> num_refw; //Э�������еľܾ�ת������

public:
	Node(int x, int y, int nodeNo, int nodeType, vector<double> data, int tolNode);
	int getNo();
	int getResource();	//��ȡ��Դ����
	int nodeType();		//��ȡ�������
	unordered_map<int, RIPRow> getRIPTable();//��ȡ·�ɱ�
	void printRIPTable();//��ӡ·�ɱ�
	void updateRIPTable(int neighbor, unordered_map<int, RIPRow> neighborRIP);//�����ھӵ�·�ɱ����·�ɱ�
	void setNeighbourNum(int heighbourNum);	//�����ھӽ����Ŀ
	void setCrossLayerAssociation(list<int> nodelist);//���ÿ�����
	void addRIPRow(RIPRow r);
	int searchRIP(int destination);//��·�ɱ���������Ҫ�ﵽdestination����һ�����
	unordered_map<int, RIPRow> getRIPtable();//��ȡ·�ɱ�
	vector<int> getCrossLayerAssociation();	//��ȡ�ý��Ŀ�����
	int getCrossLayerAssociationNum();		//��ȡ������ӵĽ����
	int getX();
	int getY();
	int getNeighbourNum();//��ȡ�ھӽڵ���Ŀ
	double getData(int require);		//������������ݣ����򷵻ظ����ݣ����򷵻�-1
	void printRes();				//��ӡ�����Դ������
	void printData();				//��ӡ�����е�����
	Packet requireData(int place, int now);	//����Χ�����������
	Packet sendData(int place, double dataVal, int des, int now);//����Ե�������ṩ����
	Packet forwardPack(Packet p, int now);	//����
	Packet receivePack(Packet p, int now, vector<double> realData);	//�հ�
	Packet refusePack(Packet p, int now);	//�ܾ��հ�
	void recover(int now);			//���ݵ�ǰʱ��now���ָ����ĵ���Դ
	void sendPacket(Packet p);		//����
	void receivePacket(Packet p);	//�հ�
	bool freeToMove(int now);		//�Ƿ��������շ���,��Դ����>10%������շ���
	void resChg(int deltaRes);		//��Դ�䶯
	void cleanpHistory();			//�����ǰ�����հ�����
	void printDataTrust();			//��Ե�����������������

	void judgeQos(int i,Packet p);//�ж��ϲ��ṩ�����Qos �ı���Ӧnum_s num_us
	void calServiceTrust();//���㵱ǰʱ��Ƭ�иýڵ�ķ�������

	void calContentTrust(int i);//���㵱ǰʱ��Ƭ�иýڵ�ԵͲ�ڵ�i����������
	bool verifySign(); //αǩ������ ���������֤�Ƿ�ɹ�

	void calHelpTrust();//���㵱ǰʱ��Ƭ�иýڵ��Э������
	
	void calRecommendTrust(int i);//�Ƽ�����

	void calComprehensiveTrust(Node n,int i);//�ۺ�����
};

#endif