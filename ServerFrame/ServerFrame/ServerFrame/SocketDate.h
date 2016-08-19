#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <list>
#include <vector>

//////////////////////////////////////////////////////////////////////////
//Class: 			header
//Description:	数据包头结构/数据包	
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

	int  version;//版本号;
	int  command;//包命令;
	int  length;//填入打包后数据部分的长度;
	int  request_id;//请求序列号;
	int  err_code;//错误码,0表示无措;
	int  reply_id;//回复序列号;
};
/*#pragma pack(pop)*/



//////////////////////////////////////////////////////////////////////////
//Class: 			SumQry
//Description:	汇总查询数据包	
//Notify:			计算服务 ==> DB
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
	long m_llUserID;		//要查询汇总的用户;
	int  m_nBillType;		//汇总查询类型：0：在线； 1：离线; 2:只差在  该字段保留;
};

//////////////////////////////////////////////////////////////////////////
//Class: 			QrySumPkg
//Description:	汇总查询返回数据结构
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
	int m_iItemCode;      //科目;
	int m_llSumFee;        //汇总费用
	int m_iBillDate;      //用于出帐查询汇总的返回,表明数据所属的日期 可以保留	;
};
typedef std::list<QrySumPkg> QrySumPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			SumQryRet
//Description:	汇总查询返回数据包	
//Notify:		   DB ==> 计算服务
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
	int m_iSumPkgCount;	//汇总数据包:CRTASumPkg 的个数;
	QrySumPkgList SumPkgMembers; 
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class:        UpSumPkg
//Description:  汇总更新汇总数据结构   
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

	long m_llUserID;			//用户ID;
	int m_iItemCode;      //科目;
	int m_iItemSrc;       //科目来源,上发固费时根据科目来源先删除再全量更新;
	int m_llSumFee;        //汇总费用;
	int m_iBillDate;      //用于出帐查询汇总的返回,表明数据所属的日期;
	int m_iReversed;       //保留字段;
	long m_llRevision;     //版本号;
};
typedef std::list<UpSumPkg> UpSumPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdate
//Description:	汇总更新数据包	
//Notify:			汇总 ==> DB
//////////////////////////////////////////////////////////////////////////
class SumUpdate
{
public:
	enum
	{
		SUM_UPDATE_REAL,    // 实时汇总
	};
	enum
	{
		UPDATE_ADD,//增量更新 
		UPDATE_ALL,//全量更新
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
	long  m_lSequence;				//唯一序列号;
	int  m_iBillDate;			//汇总数据帐期日
	int  m_iDataType;				//区分: 增量更新---RTA_UPDATE_ADD 和 全量更新---RTA_UPDATE_ALL
	//int m_iUpSumPkgCount;			//汇总数据包:CRTASumPkg的个数
	UpSumPkgList UpSumPkgMembers;	//汇总中一次只上发 汇总更新汇总数据结构 链表的size <= 20;
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateQryRet
//Description:	汇总更新返回数据包	
//Notify:			DB ==> 汇总
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
	bool state;			//标识该用户是否更新成功;
	long m_lSequence;	//唯一序列号;
	int  m_iBillDate;	//更新日期;
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class:        UpSumPkg
//Description:  计算服务更新总账数据结构   
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

	long m_llUserID;		//用户ID;
	int  m_iItemCode;      //科目;
	int  m_llSumFee;        //汇总费用;
	int  m_iBillDate;      //用于出帐查询汇总的返回,表明数据所属的日期	;
	int  m_iReversed;       //保留字段;
	int  m_llRevision;     //版本号;
};
typedef std::list<UpSumMonPkg> UpSumMonPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateMon
//Description:	    计算服务更新总帐数据包
//Notify:			计算服务 ==> DB
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
	long    m_llUserID;		//用户ID;
	int 	m_iBillDate;		//汇总数据帐期日;
	int 	m_iRegionCode;		//地市代码:571,572,等等;
	int 	m_iDataType;		//区分: 增量更新---RTA_UPDATE_ADD 和 全量更新---RTA_UPDATE_ALL;
	UpSumMonPkgList UpSumPkgMembers; 
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateMonRet
//Description:	    DB SERVER 更新总帐返回包
//Notify:			DB ==> 计算服务
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
	long m_llUserID;		//用户ID;
	bool state;			//标识该用户是否更新成功;
	int  m_iBillDate;	//更新日期;
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateNotice
//Description:	汇总通知计算服务出账数据包 	
//Notify:			汇总==>计算服务
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
	long  m_lSequence;	//唯一序列号;
	long m_llUserID;	//汇总更新的用户;
	int  m_iBillDate;      //更新日期;

};

//////////////////////////////////////////////////////////////////////////
//Class: 			SumUpdateNoticeRet
//Description:	计算服务出账返回数据包	
//Notify:			计算服务==>汇总
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
	long m_llUserID;		//用户ID;
	bool state;			//标识该用户是否更新成功;
	long  m_lSequence;	//唯一序列号;
	int m_iBillDate;	//更新日期;
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
//Class: 			CRMSumQry
//Description:	CRM总账查询数据包	
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
	long m_llUserID;		//要查询汇总的用户;
	int m_nBillType;		//汇总查询类型：0：在线； 1：离线; 2:只差在  该字段保留;
};

//////////////////////////////////////////////////////////////////////////
//Class: 			CRMQrySumPkg
//Description:	CRM总账查询返回数据结构
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
	int  m_iItemCode;      //科目;
	int  m_llSumFee;   //汇总费用;
	int  m_iBillDate;       //用于出帐查询汇总的返回,表明数据所属的日期 可以保留	;
};
typedef std::list<CRMQrySumPkg> QrySummonPkgList;

//////////////////////////////////////////////////////////////////////////
//Class: 			CRMSumQryRet
//Description:	CRM总账查询返回数据包	
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
	//	int m_iSumPkg2Count;	//汇总数据包:CRTASumPkg2 的个数
	QrySummonPkgList SumPkgMembers; 
protected:
private:
};

//////////////////////////////////////////////////////////////////////////
//Class: 			UserInfoDate
//Description:	汇总请求用户数据包
//Notify:		  汇总 ==> DB
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
	int  request_id;//请求序列号;
};

//////////////////////////////////////////////////////////////////////////
//Class: 			UserItem
//Description:	汇总请求用户返回数据结构	
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
//Description:	汇总请求用户返回数据包
//Notify:			DB ==> 汇总
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