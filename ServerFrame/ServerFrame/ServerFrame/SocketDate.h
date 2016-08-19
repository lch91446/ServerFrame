#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <list>
#include <vector>

//////////////////////////////////////////////////////////////////////////
//Class: 			header
//Description:	���ݰ�ͷ�ṹ/���ݰ�	
//Notify:			
//////////////////////////////////////////////////////////////////////////
//#pragma pack(push)

struct Header
{
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(version);
		archive & BOOST_SERIALIZATION_NVP(command);
		archive & BOOST_SERIALIZATION_NVP(length);
		archive & BOOST_SERIALIZATION_NVP(request_id);
		archive & BOOST_SERIALIZATION_NVP(err_code);
		archive & BOOST_SERIALIZATION_NVP(reply_id);
	}
	

	Header(): version(0x00000100),command(0),length(0),request_id(0),
		err_code(0),reply_id(0){};

	Header(int com): version(0x00000100),command(com),length(0),request_id(0),
		err_code(0),reply_id(0){};  

	Header(int com,int len): version(0x00000100),command(com),length(len) ,
		request_id(0),err_code(0),reply_id(0){};

	int  version;//�汾��;
	int  command;//������;
	int  length;//�����������ݲ��ֵĳ���;
	int  request_id;//�������к�;
	int  err_code;//������,0��ʾ�޴�;
	int  reply_id;//�ظ����к�;
};
/*#pragma pack(pop)*/



//////////////////////////////////////////////////////////////////////////
//Class: 			SumQry
//Description:	���ܲ�ѯ���ݰ�	
//Notify:			������� ==> DB
//////////////////////////////////////////////////////////////////////////
class SumQry
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_nBillType);
	}
	SumQry(): m_llUserID(0),m_nBillType(0){}

public:
	long m_llUserID;		//Ҫ��ѯ���ܵ��û�;
	int  m_nBillType;		//���ܲ�ѯ���ͣ�0�����ߣ� 1������; 2:ֻ����  ���ֶα���;
};

//////////////////////////////////////////////////////////////////////////
//Class: 			QrySumPkg
//Description:	���ܲ�ѯ�������ݽṹ
//Notify:		
//////////////////////////////////////////////////////////////////////////

struct QrySumPkg
{
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iItemCode);
		archive & BOOST_SERIALIZATION_NVP(m_llSumFee);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
	}
	QrySumPkg()
		:m_llSumFee(0),m_iBillDate(0),m_iItemCode(0),m_llUserID(0){};

	long m_llUserID;
	int m_iItemCode;      //��Ŀ;
	int m_llSumFee;        //���ܷ���
	int m_iBillDate;      //���ڳ��ʲ�ѯ���ܵķ���,������������������ ���Ա���	;
};
typedef std::list<QrySumPkg> QrySumPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			SumQryRet
//Description:	���ܲ�ѯ�������ݰ�	
//Notify:		   DB ==> �������
//////////////////////////////////////////////////////////////////////////
class SumQryRet
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
		archive & BOOST_SERIALIZATION_NVP(m_iSumPkgCount);
		archive & BOOST_SERIALIZATION_NVP(SumPkgMembers);
	}
public:
	SumQryRet():m_llUserID(0),m_iBillDate(0),m_iSumPkgCount(0){};
public:
	long m_llUserID;
	int m_iBillDate;
	int m_iSumPkgCount;	//�������ݰ�:CRTASumPkg �ĸ���;
	QrySumPkgList SumPkgMembers; 
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class:        UpSumPkg
//Description:  ���ܸ��»������ݽṹ   
//
//////////////////////////////////////////////////////////////////////////
struct UpSumPkg
{
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iItemCode);
		archive & BOOST_SERIALIZATION_NVP(m_iItemSrc);
		archive & BOOST_SERIALIZATION_NVP(m_llSumFee);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
		archive & BOOST_SERIALIZATION_NVP(m_iReversed);
		archive & BOOST_SERIALIZATION_NVP(m_llRevision);
	}
	UpSumPkg()
			:m_llUserID(0),m_iItemCode(0),m_iItemSrc(0)
			,m_llRevision(0),m_llSumFee(0),m_iReversed(0)
			,m_iBillDate(0){};

	long m_llUserID;			//�û�ID;
	int m_iItemCode;      //��Ŀ;
	int m_iItemSrc;       //��Ŀ��Դ,�Ϸ��̷�ʱ���ݿ�Ŀ��Դ��ɾ����ȫ������;
	int m_llSumFee;        //���ܷ���;
	int m_iBillDate;      //���ڳ��ʲ�ѯ���ܵķ���,������������������;
	int m_iReversed;       //�����ֶ�;
	long m_llRevision;     //�汾��;
};
typedef std::list<UpSumPkg> UpSumPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdate
//Description:	���ܸ������ݰ�	
//Notify:			���� ==> DB
//////////////////////////////////////////////////////////////////////////
class SumUpdate
{
public:
	enum
	{
		SUM_UPDATE_REAL,    // ʵʱ����
	};
	enum
	{
		UPDATE_ADD,//�������� 
		UPDATE_ALL,//ȫ������
	};
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_lSequence);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
		archive & BOOST_SERIALIZATION_NVP(m_iDataType);
		archive & BOOST_SERIALIZATION_NVP(UpSumPkgMembers);
	}
public:
	SumUpdate():m_lSequence(0),m_iBillDate(0),m_iDataType(0){};
	long  m_lSequence;				//Ψһ���к�;
	int  m_iBillDate;			//��������������
	int  m_iDataType;				//����: ��������---RTA_UPDATE_ADD �� ȫ������---RTA_UPDATE_ALL
	//int m_iUpSumPkgCount;			//�������ݰ�:CRTASumPkg�ĸ���
	UpSumPkgList UpSumPkgMembers;	//������һ��ֻ�Ϸ� ���ܸ��»������ݽṹ �����size <= 20;
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateQryRet
//Description:	���ܸ��·������ݰ�	
//Notify:			DB ==> ����
//////////////////////////////////////////////////////////////////////////

class SumUpdateRet
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(state);
		archive & BOOST_SERIALIZATION_NVP(m_lSequence);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
	}
public:
	SumUpdateRet():state(false),m_lSequence(0),m_iBillDate(0){};
public:
	bool state;			//��ʶ���û��Ƿ���³ɹ�;
	long m_lSequence;	//Ψһ���к�;
	int  m_iBillDate;	//��������;
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class:        UpSumPkg
//Description:  �����������������ݽṹ   
//
//////////////////////////////////////////////////////////////////////////
struct UpSumMonPkg
{
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iItemCode);
		archive & BOOST_SERIALIZATION_NVP(m_llSumFee);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
		archive & BOOST_SERIALIZATION_NVP(m_iReversed);
		archive & BOOST_SERIALIZATION_NVP(m_llRevision);
	}
	UpSumMonPkg()
		:m_llUserID(0),m_iItemCode(0),m_llSumFee(0)
		,m_iBillDate(0),m_iReversed(0),m_llRevision(0){};

	long m_llUserID;		//�û�ID;
	int  m_iItemCode;      //��Ŀ;
	int  m_llSumFee;        //���ܷ���;
	int  m_iBillDate;      //���ڳ��ʲ�ѯ���ܵķ���,������������������	;
	int  m_iReversed;       //�����ֶ�;
	int  m_llRevision;     //�汾��;
};
typedef std::list<UpSumMonPkg> UpSumMonPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateMon
//Description:	    �����������������ݰ�
//Notify:			������� ==> DB
//////////////////////////////////////////////////////////////////////////
class SumUpdateMon
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
		archive & BOOST_SERIALIZATION_NVP(m_iRegionCode);
		archive & BOOST_SERIALIZATION_NVP(m_iDataType);
		archive & BOOST_SERIALIZATION_NVP(UpSumPkgMembers);
	}
public:
	enum
	{
		UPDATE_ADD,
		UPDATE_ALL
	};
public:
	SumUpdateMon()
		:m_llUserID(0),m_iBillDate(0)
		,m_iRegionCode(0),m_iDataType(0){};
public:
	long    m_llUserID;		//�û�ID;
	int 	m_iBillDate;		//��������������;
	int 	m_iRegionCode;		//���д���:571,572,�ȵ�;
	int 	m_iDataType;		//����: ��������---RTA_UPDATE_ADD �� ȫ������---RTA_UPDATE_ALL;
	UpSumMonPkgList UpSumPkgMembers; 
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateMonRet
//Description:	    DB SERVER �������ʷ��ذ�
//Notify:			DB ==> �������
//////////////////////////////////////////////////////////////////////////
class SumUpdateMonRet
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(state);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
	}
public:
	SumUpdateMonRet():m_llUserID(0),state(false),m_iBillDate(0){};
public:
	long m_llUserID;		//�û�ID;
	bool state;			//��ʶ���û��Ƿ���³ɹ�;
	int  m_iBillDate;	//��������;
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateNotice
//Description:	����֪ͨ�������������ݰ� 	
//Notify:			����==>�������
//////////////////////////////////////////////////////////////////////////
class SumUpdateNotice
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_lSequence);
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
	}
	SumUpdateNotice():m_iBillDate(0),m_llUserID(0),m_lSequence(0){};

public:
	//typedef std::list<long> UserIdList;
	//UserIdList m_userlist;	
	long  m_lSequence;	//Ψһ���к�;
	long m_llUserID;	//���ܸ��µ��û�;
	int  m_iBillDate;      //��������;

};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateNoticeRet
//Description:	���������˷������ݰ�	
//Notify:			�������==>����
//////////////////////////////////////////////////////////////////////////
class SumUpdateNoticeRet
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(state);
		archive & BOOST_SERIALIZATION_NVP(m_lSequence);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
	}
public:
	SumUpdateNoticeRet():m_llUserID(0),state(false),m_lSequence(0),m_iBillDate(0){};
public:
	long m_llUserID;		//�û�ID;
	bool state;			//��ʶ���û��Ƿ���³ɹ�;
	long  m_lSequence;	//Ψһ���к�;
	int m_iBillDate;	//��������;
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
//Class: 			CRMSumQry
//Description:	CRM���˲�ѯ���ݰ�	
//Notify:			CRM ==> DB
//////////////////////////////////////////////////////////////////////////
class CRMSumQry
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_nBillType);
	}
	CRMSumQry(): m_nBillType(0),m_llUserID(0){}

public:
	long m_llUserID;		//Ҫ��ѯ���ܵ��û�;
	int m_nBillType;		//���ܲ�ѯ���ͣ�0�����ߣ� 1������; 2:ֻ����  ���ֶα���;
};

//////////////////////////////////////////////////////////////////////////
//Class: 			CRMQrySumPkg
//Description:	CRM���˲�ѯ�������ݽṹ
//Notify:		
//////////////////////////////////////////////////////////////////////////

struct CRMQrySumPkg
{
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_iItemCode);
		archive & BOOST_SERIALIZATION_NVP(m_llSumFee);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
	}

	CRMQrySumPkg():m_iBillDate(0),m_iItemCode(0),m_llSumFee(0){};
	int  m_iItemCode;      //��Ŀ;
	int  m_llSumFee;   //���ܷ���;
	int  m_iBillDate;       //���ڳ��ʲ�ѯ���ܵķ���,������������������ ���Ա���	;
};
typedef std::list<CRMQrySumPkg> QrySummonPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			CRMSumQryRet
//Description:	CRM���˲�ѯ�������ݰ�	
//Notify:			DB ==> CRM
//////////////////////////////////////////////////////////////////////////
class CRMSumQryRet
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(m_llUserID);
		archive & BOOST_SERIALIZATION_NVP(m_iBillDate);
		archive & BOOST_SERIALIZATION_NVP(SumPkgMembers);
	}
public:
	CRMSumQryRet():m_llUserID(0),m_iBillDate(0){};
public:
	long m_llUserID;
	int  m_iBillDate;
	//	int m_iSumPkg2Count;	//�������ݰ�:CRTASumPkg2 �ĸ���
	QrySummonPkgList SumPkgMembers; 
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			UserInfoDate
//Description:	���������û����ݰ�
//Notify:		  ���� ==> DB
//////////////////////////////////////////////////////////////////////////
class UserInfoDateQry
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(request_id);
	}
	UserInfoDateQry():request_id(0){};
	int  request_id;//�������к�;
};

//////////////////////////////////////////////////////////////////////////
//Class: 			UserItem
//Description:	���������û��������ݽṹ	
//Notify:		
//////////////////////////////////////////////////////////////////////////
struct UserItem
{
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(user_id);
// 		archive & BOOST_SERIALIZATION_NVP(acct_id);
// 		archive & BOOST_SERIALIZATION_NVP(cust_id);
// 		archive & BOOST_SERIALIZATION_NVP(region_code);
// 		archive & BOOST_SERIALIZATION_NVP(status);
// 		archive & BOOST_SERIALIZATION_NVP(fee);
	}
	UserItem():user_id(0){};
// 		:user_id(0),acct_id(0),cust_id(0)
// 		,region_code(0),status(0),fee(0){};
	long user_id;
// 	long acct_id;
// 	long cust_id;
// 	int region_code;
// 	int status;
// 	int fee;
};
typedef std::vector<UserItem> UserItemList;

//////////////////////////////////////////////////////////////////////////
//Class: 			UserInfoDate
//Description:	���������û��������ݰ�
//Notify:			DB ==> ����
//////////////////////////////////////////////////////////////////////////
class UserInfoDateRet
{
public:
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(UserItems);
	}
	
	UserItemList UserItems;
};