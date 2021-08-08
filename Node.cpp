#include <algorithm>
#include "Node.h"
#include "packet.h"
#define oo 9999999
#define R 0.5
#define N 100
const int minpoints = 10;


Node::Node(int x, int y, int nodeNo, int nodeType, vector<double> data, int tolNode)
{
	this->no = nodeNo;
	set<int> s;
	this->pHistory = s;
	this->type = nodeType;

	this->x = x;
	this->y = y;

	//�۲�����
	map<int, double> a;
	if (type < 0) {//��Ե��������ȡ��ʼ����
		srand((unsigned)clock());
		for (int i = 0; i < DATA_TYPE ; i++) {
			srand((unsigned)clock());
			//����Ӽ�[-0.99, 1],ģ��۲����
			a[i] = data[i] + 1.0*(random(20) - 9) / 10;
			if (type == MALICIOUS_EDGE)
				a[i] += 1.0*(random(200) - 99) / 10;
			Sleep(1);
		}
		
	}
	else {//�����۲�����
		for (int i = 0; i < random(DATA_TYPE); i++) {
			srand((unsigned)clock());
			int place = random(DATA_TYPE);
			if (!this->data.count(place)) {
				a[place] = data[place] + 1.0*(random(200) - 99) / 10;
			}
			Sleep(1);
		}
	}
	this->data = a;
	Sleep(1);
	srand((unsigned)clock());
	//����Ĭ����Դ��
	if (nodeType < 0)
		//��Ե��������ʼ��Դ��Χ[150, 1000)
		this->resource = random(EDGE_RES - 150) + 150;
	else
		//������ʼ��Դ��Χ[15,100)
		this->resource = random(THING_RES - 15) + 15;

	//��ʼ����������,������������¼��ֵ
	this->dataTrust.assign(tolNode, INIT_DATA_TRUST);

	//��ʼ����Ե�����������ݻ�����
	if (this->type < 0) {
		//���������������ṩ�����ݣ������ж����������Ժ�Ŵ����Լ���������
		//��ʵDATA_TYPE��ThingNodeNum�Ĵ�С�͹�
		vector<vector<double> > t(DATA_TYPE, vector<double>(tolNode, NoDATA));
		this->dataBuffer = t;
		this->dataTimeLimit.assign(DATA_TYPE, 0);
	}
}

int Node::getNo()
{
	return no;
}

int Node::getResource()
{
	return resource;
}

int Node::nodeType ()
{
	return type;
}

unordered_map<int, RIPRow> Node::getRIPTable()
{
	return this->ripTable;
}

void Node::printRIPTable()
{
	cout << this->no << "����RIP·�ɱ�\n";
	cout << "Ŀ�ĵ�ַ|��һ��|����\n";
	for(auto &i : this->ripTable)
		cout << i.second.getDestination() << '\t'
			<< i.second.getNextNode() << '\t'
			<< i.second.getDistance() << '\n';
}

void Node::updateRIPTable(int neighbor, unordered_map<int, RIPRow> neighborRIP)
{
	//�����ھӵ�·�ɱ�����Լ���·�ɱ�
	for (unordered_map<int, RIPRow>::iterator it = neighborRIP.begin();it != neighborRIP.end(); it++) 
    {
		RIPRow rip = it->second;
		auto myRip = ripTable.find(it->first);		//���Լ���·����������Ŀ�ĵ�ַ
		if (rip.getDistance() + 1 == OUT_OF_RANGE)	//���ɴ�ļ�¼
			continue;
		if (rip.getDestination() == this->no)		//����¼����
			continue;
		if (myRip != ripTable.end()) {
			//�Ѿ����е�����һĿ�ĵ�ַ�ļ�¼,ȡ����С��
			int dis = rip.getDistance();
			if (dis + 1 < myRip->second.getDistance()) {
				rip.addOneJumpDistance();
				rip.setNextNode(neighbor);
				myRip->second = rip;
			}
		}
		else {
			//��δ��¼�Ľ�㣬����·�ɱ�
			rip.addOneJumpDistance();
			rip.setNextNode(neighbor);
			this->ripTable.insert(pair<int, RIPRow>(it->first, rip));
		}
	}
}

void Node::setNeighbourNum(int heighbourNum)
{
	this->neighbourNum = neighbourNum;
}

void Node::setCrossLayerAssociation(list<int> nodelist)
{
	//���ÿ�������㣬�����������������¼��Χ�ڵ����
	for (list<int>::iterator it = nodelist.begin(); it != nodelist.end(); it++)
		this->crossLayerAssociation.push_back(*it);
}

void Node::addRIPRow(RIPRow r)
{
	//���һ�������¼
	this->ripTable.insert(pair<int, RIPRow>(r.getDestination(), r));
}

int Node::searchRIP(int destination)
{
	//����destination����һ����㣬��·�ɱ���û�е�des�ļ�¼����-1
	for (unordered_map<int, RIPRow>::iterator it = this->ripTable.begin();
		it != this->ripTable.end(); it++) {
		if (it->second.getDestination() == destination
			&& it->second.getDistance() < OUT_OF_RANGE)
			return it->second.getNextNode();
	}
	return -1;
}

unordered_map<int, RIPRow> Node::getRIPtable()
{
	return this->ripTable;
}

vector<int> Node::getCrossLayerAssociation()
{//��ȡ�ý��Ŀ�����������������Χ�ڵ���������������ӵķ�����
	return this->crossLayerAssociation;
}

int Node::getCrossLayerAssociationNum()
{
	return this->crossLayerAssociation.size();
}

int Node::getX()
{
	return this->x;
}

int Node::getY()
{
	return this->y;
}

int Node::getNeighbourNum()
{
	return this->neighbourNum;
}

double Node::getData(int require)
{
	//������������ݣ����򷵻ظ����ݣ����򷵻�-1
	if (data.count(require))
		return data[require];
	return DATA_NOT_FOUND;
}

void Node::printRes()
{
	//��ӡ�����Դ������
	cout << no << "  ������ͣ�";
	if (type < 0)
		cout << "��Ե������\n  ��Դ����ռ�ȣ�"<<100.0*resource/EDGE_RES << "%\n";
	else
		cout << "�����\n  ��Դ����ռ�ȣ�" << 100.0*resource / THING_RES << "%\n";
}

void Node::printData()
{
	//��ӡ�����е�����
	printf("%d��%d�ڵ���е�����������£�================\n", this->no,this->type);
	for (map<int, double>::iterator it = this->data.begin(); it != this->data.end(); it++) {
		printf("  %d������Ϊ%f\n", it->first, it->second);
	}
}

Packet Node::requireData(int place, int now)
{
	//����Χ�ڵ��������ݣ�des==-1��ʾ�㲥��
	this->resChg((-1)*SEND_PACK_CONSUME);
	Packet p(this->no, -1, REQ_DATA, now);
	map<int, double> d;
	d.insert(make_pair(place, -1));
	p.setPackBody(d);
	this->sendBuffer.push(p);
	this->pHistory.insert(p.getPno());
	return p;
	////�������������
	//if (this->type > 0) {
	//	Packet p(this->no, -1, REQ_DATA, now);
	//	
	//}
	//else {//��Ե��������������
	//	Packet p(this->no, -1, REQ_DATA, now);
	//}
}

Packet Node::sendData(int place, double dataVal, int des, int now)
{
	Packet p(this->no, des, ANS_DATA, now);
	map<int, double> d;
	d.insert(make_pair(place, dataVal));
	p.setPackBody(d);
	this->sendBuffer.push(p);
	this->pHistory.insert(p.getPno());
	return p;
}

Packet Node::forwardPack(Packet p, int now)
{
	//��Դ������
	if (!this->freeToMove(now)) {
		return this->refusePack(p, now);
	}

	//������ܾ�ת��
	Sleep(1);
	srand((unsigned)clock());
	if (this->type == MALICIOUS_THING && 1.0*random(100) / 100 < F) {
		return this->refusePack(p, now);
	}

	//ת��
	this->sendPacket(p);
	p.setLastSender(p.getSender());
	p.setSender(this->no);
	p.chgBirth(now + p.getDelay());//randomģ�⴦����շ���ʱ��
#ifdef DEBUG
	printf("%d���ת������%d����%d�Ű�\n", p.getSender(), p.getLastSender(), p.getPno());
#endif
	return p;
}

Packet Node::receivePack(Packet p, int now, vector<double> realData)
{
	//�Ѿ��չ������������������Լ�����������������ؿհ���ʾ���������������
	//Ŀǰ������δ�������ض������������������Ĭ���������ݵİ�Ŀ�ĵ�ַ����-1
	if (this->pHistory.find(p.getPno()) != this->pHistory.end() ||
		(p.getSource() == this->no && p.getPackType() == REQ_DATA)) {
		return Packet();
	}

	this->pHistory.insert(p.getPno());
	map<int, double> d = p.getData();

	//�����Լ���Ӧ�������������Դ����
	if (p.getDestination() == this->no && p.getPackType() == ANS_DATA){
		//��Ե�������հ�
		if (this->type < 0) {


			
		}
		//������հ�
		else {
			for (map<int, double>::iterator it = d.begin();
				it != d.end(); it++) {
				int place = (*it).first;
				double val = (*it).second;
				if (abs(realData[place] - val) < 1.1) {
					this->dataTrust[p.getSource()]++;
					this->data[(*it).first] = (*it).second;
				}
			}
		}
		return Packet();
	}

	//����п�����Դ����Ӧ���
	else if (this->freeToMove(now) 
		&& p.getPackType() == ANS_DATA) {
		this->resChg((-1)*RECEIVE_PACK_CONSUME);
		this->receiveBuffer.push(p);
		return this->forwardPack(p, now);
	}

	//�п�����Դ���������
	if (this->freeToMove(now) && p.getPackType() == REQ_DATA) {
		this->receivePacket(p);

		//��Ӧ��������
		if (p.getDestination() == -1 ||
			(p.getDestination() == no && p.getPackType() == REQ_DATA)) {
			bool flag = 0;//��������
			for (map<int, double>::iterator it = d.begin();
				it != d.end(); it++) {
				double myData = this->getData((*it).first);
				if (myData != DATA_NOT_FOUND) {
					flag = 1;
					(*it).second = myData;
#ifdef DEBUG
					printf("%d��㷢��%d������%f��%d\n",
						no, (*it).first, (*it).second, p.getSource());
#endif
				}
			}
			//����������
			if (flag) {
				Packet ans(this->no, p.getSource(), ANS_DATA, now);
				ans.setPackBody(d);
				ans.chgBirth(now + ans.getDelay());//ģ�⴦��ʱ��
				return ans;
			}
			//û����������ݣ������ת��
			if (this->type > 0)
				return this->forwardPack(p, now);
		}
		//�����˵�Ӧ�����
		else {
			if (this->no > 0)	//�����ת��
				return this->forwardPack(p, now);
			else				//��Ե������������
				return Packet();
		}
	}
	//�޿�����Դ�ܾ��հ�==�ܾ�ת����
	return this->refusePack(p, now);
}

Packet Node::refusePack(Packet p, int now)
{
	this->pHistory.erase(p.getPno());
	//��һ��delayģ�⴦��ʱ��
	return Packet(this->no, p.getSender(), REFUSEFORWARD, now + p.getDelay());
}

void Node::recover(int now)
{
	//����ָ�һ��������Դ
	this->resChg(random(25));

	//��������������
	while (!sendBuffer.empty() &&
		sendBuffer.front().getBirth() + sendBuffer.front().getDelay()
		>= now) {
		this->resChg(SEND_PACK_CONSUME);
		sendBuffer.pop();
	}
	//�հ�����������
	while (!receiveBuffer.empty() &&
		receiveBuffer.front().getBirth() + receiveBuffer.front().getDelay()
		>= now) {
		this->resChg(RECEIVE_PACK_CONSUME);
		receiveBuffer.pop();
	}
}

void Node::sendPacket(Packet p)
{
	this->sendBuffer.push(p);
	this->resChg((-1)*SEND_PACK_CONSUME);
}

void Node::receivePacket(Packet p)
{
	this->resChg((-1)*RECEIVE_PACK_CONSUME);
	this->receiveBuffer.push(p);
}

bool Node::freeToMove(int now)
{
	this->recover(now);

	//��Դ���� > 10% ������շ���
	if (type < 0 && this->resource > EDGE_RES / 10)
		return true;
	if (type > 0 && this->resource > THING_RES / 10)
		return true;
	return false;
}


void Node:: resChg(int deltaRes)
{
	this->resource += deltaRes;
}
void Node::cleanpHistory()
{
	//�����ǰ�����հ�����
	this->pHistory.clear();
}

void Node::printDataTrust()
{
	int n = this->dataTrust.size();
	for (int i = 0; i < n; i++) {
		printf("%f ", this->dataTrust[i]);
	}
	printf("\n");
}

void judgeQos(int i,Packet p)
{
	if((p.getDelay()>SAT_DELAY)&&(p.getBandwidth()>SAT_BANDWIDTH) num_s[i]++;
	else num_us[i]++;
}

void Node::calServiceTrust(int i)
{
	Ts[i]=(num_s[i]+1)/(num_s[i]+num_us[i]+2);
}

void Node::calContentTrust(int i)
{
	Tc[i]=(num_c[i]+1)/(num_c[i]+num_uc[i]+2);
}

void Node::calHelpTrust(int i)
{
	Th[i]=(num_fw[i]+1)/(num_fw[i]+num_refw[i]+2);
}

void Node::calComprehensiveTrust(Node n,int i)//�������� ��ͬ����豸��ֱ�����β�ͬ
{
	if()
}