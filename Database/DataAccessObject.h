#pragma once
#include "../Logic/Packets.h"
#include <QtSql>
#include <vector>
#include <QMutex>

class DataAccessObject
{

    QSqlDatabase* db;
    QString login;
    QString password;
    QMutex mutex;

public:
	ResponseAddUserPacket addUser(AddUserPacket userToAdd);
    ResponseCheckRestaurantPacket findNewestRestaurant();
    std::vector<ResponseGetRestaurantPacket> getRestaurant(GetRestaurantPacket);
    std::vector<ResponseGetCommentsPacket> getComment(GetCommentsPacket);
	ResponseAddCommentPacket addComment(AddCommentPacket);
	ResponseAddRestaurantPacket addRestaurant(AddRestaurantPacket);
	ReponseDeleteCommentPacket deleteComment(DeleteCommentPacket);


	DataAccessObject(void);
	~DataAccessObject(void);
};

