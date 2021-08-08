#include<map>
#include<ctime>
#include<fstream>
using namespace std;
#define random(x) (rand()%x)	//[0, x)�����ֵ
const string SCALE_PATH = "scale.txt";//�����ģ�ļ����洢���ͱ�Ե��������
const int REQ_DATA = 0;	//�����
const int ANS_DATA = 1;	//Ӧ���
const int RECEIVE_PACK_CONSUME = 17;	//�հ�������Դ��
const int SEND_PACK_CONSUME = 13;	//����������Դ��
const int EMPTY_PACK = -10;			//�հ����
class Packet {
private:
	static int pnum;		//����
	int pno;				//����
	int source;				//Դ��ַ
	int destination;		//Ŀ�ĵ�ַ��-1��ʾ�㲥��
	double delay;				//�շ���ʱ��
	double birth;				//��������ʱ��
	double bandwidth;           //���ʹ���
	int num_Qos;            //Qos��������
	map<int, double> packBody;	//���壬���ݰ��ľ�������
	int packType;			//0��ʾ�������ݣ�1��ʾ��������,2��ʾ��������
	int lastSender;			//ǰһ���������
	int sender;				//����������Ľ�㣬���ǰ���Դ��ַ
	int type;				//�����Բ�����࣬2��ʾ�Ʋ�,1��ʾ��Ե������,0��ʾ�����

public:

	Packet();

	Packet(int sou, int des, int packType, int birTime);


	void setSender(int s);

	int getSender();

	void setLastSender(int s);

	int getLastSender();

	void setPackBody(map<int, double> data);

	int getPno();

	int getSource();

	int getDestination();

	int getPackType();

	int getBirth();

	int getDelay();

	int getBandwidth();
	
	map<int, double> getData();

	void printPacket();//��ӡ��������


};

