#include "MyProject.h"
#include "NetTCPClient.h"
#include "ClientBuffer.h"

#ifdef USE_EXTERN_THREAD

NetTCPClient::NetTCPClient(ISocketHandler& h)
	: TcpSocket(h) 
{
	this->mClientBuffer = new ClientBuffer();
}

NetTCPClient::~NetTCPClient()
{
	delete this->mClientBuffer;
}

void NetTCPClient::OnConnect()
{
	this->Send("quit\n");
}

void NetTCPClient::OnConnectFailed()
{
	this->SetCloseAndDelete();
}

void NetTCPClient::OnDelete()
{
	
}

void NetTCPClient::OnRawData(const char *buf, size_t len)
{

}

ClientBuffer* NetTCPClient::getClientBuffer()
{
	return this->mClientBuffer;
}

void NetTCPClient::sendMsg()
{
	//mClientBuffer->sendMsg();
}

#endif	// USE_EXTERN_THREAD