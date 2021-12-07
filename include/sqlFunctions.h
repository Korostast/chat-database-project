#ifndef CHATDATABASEPROJECT_SQLFUNCTIONS_H
#define CHATDATABASEPROJECT_SQLFUNCTIONS_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QDate>
#include "UserInfo.h"
#include "ChatWidget.h"
#include "ChatDialog.h"
#include "AvatarEditor.h"
#include "Defines.h"
#include "CustomPlainTextEdit.h"
#include "UserMessageWidget.h"

#include <QSqlDatabase>
#include <QSqlQuery>

UserInfo sqlAuthenticate(QString handle, QString password_hash);


#endif //CHATDATABASEPROJECT_SQLFUNCTIONS_H