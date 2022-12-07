#ifndef MEMBER_H
#define MEMBER_H

#include <QDate>

enum MemberType {REGULAR, EXECUTIVE};
const float REBATE_RATE = 0.04;

class Member
{
public:

    Member(QString name, int id, int renewCost){
        memberName      = name;
        memberID        = id;
        memberRenewal   = renewCost;
        rebate          = 0.00;
        totalSpent      = 0.00;
    }

    Member(QString name, int id, float rebateAmt){
        memberName = name;
        memberID   = id;
        rebate     = rebateAmt;
        totalSpent = 0.0;
    }


    Member(QString name, int id, MemberType mType, QDate expDate){
        memberName      = name;
        memberID        = id;
        membership      = mType;
        expirationDate  = expDate;
        rebate          = 0.00;
        totalSpent      = 0.00;
    }

    Member(QString name, int id, MemberType mType, QDate expDate, float rebateAmt, float totSpent){
        memberName      = name;
        memberID        = id;
        membership      = mType;
        expirationDate  = expDate;
        rebate          = rebateAmt;
        totalSpent      = totSpent;
    }

    QString GetMemberName() const{return memberName;}

    int GetID() const{return memberID;}

    QString GetMembershipTypeString() const{
        QString membershipType;

        switch(membership)
        {
        case REGULAR:
            membershipType = "Regular";
            break;
        case EXECUTIVE:
            membershipType = "Executive";
            break;
        };

        return membershipType;
    }


    QString GetExpirationDateString() const{return expirationDate.toString("MM/dd/yyyy");}

    float GetRebate() const{return rebate;}

    float GetTotalSpent() const{return totalSpent;}

    int GetRenewCost() const{return memberRenewal;}


    bool isExecutive() const{
        bool executiveMember;

        if(membership == EXECUTIVE)
        {
            executiveMember = true;
        }
        else
        {
            executiveMember = false;
        }

        return executiveMember;
    }
private:
    QString     memberName;
    int         memberID;
    MemberType  membership;
    QDate       expirationDate;
    float       rebate;
    float       totalSpent;
    int         memberRenewal;
};

#endif // MEMBER_H
