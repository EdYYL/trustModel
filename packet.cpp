#include "Packet.h"
int Packet::pnum;//���������������Զ����ɰ���

//void Packet::setBirth(int time)
//{	//���ð�������ʱ��
//	birth = time;
//}

Packet::Packet()
{
	this->source = EMPTY_PACK;
	this->destination = EMPTY_PACK;
	this->packType = EMPTY_PACK;
}

Packet::Packet(int sou, int des, int type, int birTime)
{
	this->pno = ++pnum;
	this->source = sou;
	this->destination = des;
	this->packType = type;
	ifstream fin(SCALE_PATH, ios::in);
	int thingNum;
	fin >> thingNum;
	//��ʼ��һ��û�����ݵĿհ�
	map<int, double> a;
	this->packBody = a;

	//��ʼ��ʱ����[5, 20)֮��
	srand((unsigned)clock());
	this->delay = random(15) + 5;

	//���ʱ��
	this->birth = birTime;

	this->sender = sou;
	this->lastSender = -1;
}

void Packet::setSender(int s)
{
	sender = s;
}

int Packet::getSender()
{
	return sender;
}

void Packet::setLastSender(int s)
{
	lastSender = s;
}

int Packet::getLastSender()
{
	return lastSender;
}

void Packet::setPackBody(map<int, double> data)
{
	//���ð���
	this->packBody = data;
}

int Packet::getPno()
{
	return pno;
}

int Packet::getSource()
{
	return source;
}

int Packet::getDestination()
{
	return destination;
}

int Packet::getPackType()
{
	return packType;
}

int Packet::getBirth()
{
	return birth;
}

int Packet::getDelay()
{
	return delay;
}

map<int, double> Packet::getData()
{
	return packBody;
}

void Packet::printPacket()
{
	printf("-----------------------------------------\n");
	printf("���ţ�%d\nԴ��ַ��%d\nĿ�ĵ�ַ��%d\nʱ�ӣ�%d\n����ʱ�䣺%d\n���ͣ�%d\n"
		,pno, source, destination, delay, birth, packType);
	printf("���壺\n");
	for (map<int, double>::iterator it = packBody.begin();
		it != packBody.end(); it++) {
		printf("%d������Ϊ%f\n", (*it).first, (*it).second);
	}
	printf("��:%d�Ž��ת����%d�Žڵ�\n", lastSender, sender);
	printf("-----------------------------------------\n");
}
