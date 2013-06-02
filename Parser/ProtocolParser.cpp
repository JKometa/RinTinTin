#include "ProtocolParser.h"
#include <iostream>
#include <string>
#include "../Exceptions/BadPackageException.h"


ProtocolParser::ProtocolParser(void)
{
}


ProtocolParser::~ProtocolParser(void)
{
}

PacketType ProtocolParser::readPacketType(int& index) {
    QString packetType;
	for(index = 0; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        packetType.append(this->data[index]);
	}
	++index;
	

    int value = (PacketType)(packetType.toInt());
    if ( value < 0 || value > 17) throw new BadPackageException();

    return (PacketType)(value);
}

AddUserPacket ProtocolParser::parseUserPacket(int index) {
    AddUserPacket temp;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' )
			throw new BadPackageException();
        temp.login.append(this->data[index]);
	}
	++index;
	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' )
			throw new BadPackageException();
        temp.password.append(this->data[index]);
	}

	return temp;
}


CheckRestaurantPacket ProtocolParser::parseCheckRestaurantPacket(int index) {
    QString temp;
	CheckRestaurantPacket packet;
	for(; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        temp.append(this->data[index]);
	}

    packet.localLastRestaurantId = temp.toInt();

	return packet;
}


GetRestaurantPacket ProtocolParser::parseGetRestaurantPacket(int index) {
    QString temp;
	GetRestaurantPacket packet;
	for(; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        temp.append(this->data[index]);
	}

    packet.toUpdateRestaurantId = temp.toInt();

	return packet;
}

GetCommentsPacket ProtocolParser::parseGetCommentsPacket(int index) {
    QString temp;
	GetCommentsPacket packet;
	for(; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        temp.append(this->data[index]);
	}

    packet.restaurantId = temp.toInt();
	++index;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' ) //TODO doprecyzowac jak bedzie wiadomo JAKI format ma data
			throw new BadPackageException();
        packet.addedDate.append(this->data[index]);
	}

	return packet;
}


AddCommentPacket ProtocolParser::parseAddCommentPacket(int index) {
    QString temp;
	AddCommentPacket packet;
	for(; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        temp.append(this->data[index]);
	}

    packet.userId = temp.toInt();
	temp.clear();
	++index;

	for(; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        temp.append(this->data[index]);
	}

    packet.restaurantId = temp.toInt();
	++index;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' ) //TODO doprecyzowac jak bedzie wiadomo JAKI format ma data
			throw new BadPackageException();
        packet.date.append(this->data[index]);
	}
	++index;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == '\0' )
			throw new BadPackageException();
        packet.text.append(this->data[index]);
	}

	return packet;
}


AddRestaurantPacket ProtocolParser::parseAddRestaurantPacket(int index) {
	AddRestaurantPacket packet;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' ) //TODO doprecyzowac jak bedzie wiadomo JAKI format ma data
			throw new BadPackageException();
        packet.restaurantName.append(this->data[index]);
	}
	++index;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' )
			throw new BadPackageException();
        packet.restaurantAdress.append(this->data[index]);
	}
	++index;

	for(; this->data[index] != '\n'; ++index) {
		if( this->data[index] == ' ' )
			throw new BadPackageException();
        packet.restaurantType.append(this->data[index]);
	}

	return packet;
}


DeleteCommentPacket ProtocolParser::parseDeleteCommentPacket(int index) {
    QString temp;
	DeleteCommentPacket packet;
	for(; this->data[index] != '\n'; ++index) {
		if(this->data[index] < '0' || this->data[index] > '9')
			throw new BadPackageException();
        temp.append(this->data[index]);
	}

    packet.commentId = temp.toInt();

	return packet;
}


PacketType ProtocolParser::parsePacketIn(char* input,int& index) {
	this->data = input;
	index = 0;
	PacketType typePacket;

	typePacket = this->readPacketType(index);

	return typePacket;
}

//////////////////////////////////////////////////////////////////Out///////////////////////////////////////////////////
QString intToStr(int n)
{
     QString tmp, ret;
     if(n < 0) {
          ret = "-";
          n = -n;
     }
     do {
          tmp += n % 10 + 48;
          n -= n % 10;
     }
     while(n /= 10);
     for(int i = tmp.size()-1; i >= 0; i--)
          ret += tmp[i];
     return ret;
}

QString ProtocolParser::parsePacketOut(ResponseAddCommentPacket packet) {
    QString response;
	response += "13";
	response += '\n';

    response += intToStr(packet.commentId);
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(ResponseAddRestaurantPacket packet) {
    QString response;
	response += "15";
	response += '\n';

	response += intToStr(packet.restaurantId);
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(ResponseAddUserPacket packet) {
    QString response;
	response += "3";
	response += '\n';

	response += intToStr(packet.userId);
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(ResponseCheckRestaurantPacket packet) {
    QString response;
	response += "5";
	response += '\n';

	response += intToStr(packet.globalLastRestaurantId);
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(ResponseGetCommentsPacket packet) {
    QString response;
	response += "11";
	response += '\n';

	response += packet.userLogin;
	response += '\n';

	response += packet.date;
	response += '\n';

	response += packet.date;
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(ResponseGetRestaurantPacket packet) {
    QString response;
	response += "7";
	response += '\n';
	
	response += intToStr(packet.restaurantId);
	response += '\n';

	response += packet.restaurantName;
	response += '\n';

	response += packet.restaurantAdress;
	response += '\n';

	response += packet.restaurantType;
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(PongPacket packet) {
    QString response;
	response += "1";
	response += '\n';

	response += intToStr(packet.userId);
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketOut(ReponseDeleteCommentPacket packet) {
    QString response;
	response += "8";
	response += '\n';
	(packet.ifDeleted == true ? response += "1" : response +="0" );
	response += '\n';

	return response;
}

QString ProtocolParser::parsePacketEndOfData() {
    QString response;
	response +="9";
	response += '\n';
	return response;
}

bool ProtocolParser::parseNextData(char* inputBuffor) {
	if(inputBuffor[0] == '8')
		return true;
	else 
		throw new BadPackageException();
}
