#include <samchon/protocol/service/Client.hpp>
#	include <samchon/protocol/service/Server.hpp>
#	include <samchon/protocol/service/User.hpp>
#	include <samchon/protocol/service/Service.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <samchon/library/SQLi.hpp>
#include <samchon/library/SQLStatement.hpp>
#include <samchon/library/XML.hpp>
#include <samchon/library/Semaphore.hpp>

#include <samchon/protocol/Invoke.hpp>

using namespace std;

using namespace boost::asio;
using namespace boost::asio::ip;

using namespace samchon;
using namespace samchon::library;
using namespace samchon::protocol;
using namespace samchon::protocol::service;

/* --------------------------------------------------------
	CONSTRUCTORS
-------------------------------------------------------- */
Client::Client(User *user, size_t no, Socket *socket)
	: super()
{
	this->user = user;
	this->no = no;

	this->socket = socket;
}
Client::~Client()
{
	delete service;
}

auto Client::__keepAlive() -> ServiceKeeper
{
	return ServiceKeeper(user, this);
}

/* --------------------------------------------------------
	GETTERS
-------------------------------------------------------- */
auto Client::getUser() const -> User*
{
	return user;
}
auto Client::getService() const -> Service*
{
	return service;
}
auto Client::getNo() const -> size_t
{
	return no;
}

/* --------------------------------------------------------
	HANDLING MESSAGE
-------------------------------------------------------- */
void Client::sendData(shared_ptr<Invoke> invoke)
{
	KEEP_CLIENT_ALIVE;

	super::sendData(invoke);
}
void Client::replyData(shared_ptr<Invoke> invoke)
{
	String &listener = invoke->getListener();

	if (listener == _T("notifyService"))
	{
		constructService(invoke->at(0)->getValue());
	}
	else if (listener == _T("login"))
	{
		user->goLogin(this, invoke);
	}
	else if (listener == _T("logout"))
	{
		user->goLogout(this);
	}
	else if (listener == _T("join"))
	{
		user->goJoin(this, invoke);
	}
	else
	{
		if (service != nullptr)
		{
			thread([this, invoke]()
			{
				KEEP_CLIENT_ALIVE;
				UniqueAcquire acquire(*user->getSemaphore());

				service->replyData(invoke);
			}).detach();
		}
	}
}

void Client::constructService(const String &name)
{
	if (service != nullptr)
		return;

	int authority;
	bool satisfactory;

	service = createService(name);
	if (service == nullptr)
	{
		authority = 0;
		satisfactory = false;
	}
	else
	{
		authority = user->getAuthority();
		satisfactory = (authority >= service->REQUIRE_AUTHORITY());
	}

	shared_ptr<Invoke> replyInvoke(new Invoke(_T("notifyAuthority")));
	replyInvoke->push_back(new InvokeParameter(_T("authority"), authority));
	replyInvoke->push_back(new InvokeParameter(_T("satisfactory"), satisfactory));

	sendData(replyInvoke);
}