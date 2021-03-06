#ifndef COMMDATASTRUCT_H
#define COMMDATASTRUCT_H

#include <QDate>
#include <QHash>

#include "cal.h"
#include "securitys.h"

class PingZheng;
class BusiAction;
class User;
class FirstSubject;
class Machine;

const int UNID      = 0;    //无意义的id值，比如对于新创建但还未保存的二级科目对象的id值
const int UNCLASS   = 0;    //未知的分类
const int ALLCLASS  = 0;    //所有类别


//变量值类型枚举
enum VariableType{
    VT_BOOL    = 1, //布尔型
    VT_INTEGER = 2, //整形
    VT_FLOAT   = 3, //浮点型
    VT_STRING  = 4  //字符串
};

//剪贴板操作
enum ClipboardOperate{
    CO_COPY     = 1,
    CO_CUT      = 2,
    CO_PASTER   =3
};

//智能提示列表框项目的排序模式
enum SortByMode{
    SM_NAME   = 1,     //按名称字符
    SM_CODE   = 2,     //按科目代码
    SM_REMCOD = 3      //按助记符
};

//应用错误级别名称
enum AppErrorLevel{
    AE_OK            = 0,       //正常信息
    AE_WARNING       = 1,       //警告信息（影响操作的顺利运行）
    AE_CRITICAL      = 2,       //临界错误（影响操作的正确结果）
    AE_ERROR         = 3        //致命错误（影响应用程序的运行，可能导致崩溃）
};

//通用项目编辑状态
enum CommonItemEditState{
    CIES_PENDING  = -1,    //未决的（比如数据库表中还没有与此对象对应的记录）
    CIES_INIT     = 0,     //初始的（从数据库表中读取并创建该对象后未做更改）
    CIES_NEW      = 1,     //新增的（对象是新增的，但在数据库表中已存在对应记录）
    CIES_CHANGED  = 2,     //改变了
    CIES_DELETED  = 3,     //删除了
    CIES_DELETEDAFTERCHANGED = 4 //修改后被删除
};

//名称条目编辑状态
enum NameItemEditState{
    ES_NI_INIT = 0x0,       //未做任何修改
    ES_NI_CLASS = 0x01,     //名称条目类别改变
    ES_NI_SNAME = 0x02,     //名称条目简称被修改
    ES_NI_LNAME = 0x04,     //名称条目全称被修改
    ES_NI_SYMBOL = 0x08     //名称条目助记符被修改

};
Q_DECLARE_FLAGS(NameItemEditStates, NameItemEditState)
Q_DECLARE_OPERATORS_FOR_FLAGS(NameItemEditStates)

//一级科目编辑状态
enum FirstSubjectEditState{
    ES_FS_INIT      = 0x0,      //未做任何修改
    ES_FS_CODE      = 0x01,     //科目代码
    ES_FS_SYMBOL    = 0x02,     //助记符
    ES_FS_CLASS     = 0x04,     //科目所属类别
    ES_FS_JDDIR     = 0x08,     //记账方向
    ES_FS_ISENABLED = 0x10,     //是否启用
    ES_FS_ISUSEWB   = 0x20,     //是否使用外币
    ES_FS_WEIGHT    = 0x40,     //权重
    ES_FS_NAME      = 0x80,     //科目名称
    ES_FS_DESC      = 0x100,    //简要描述
    ES_FS_USAGE     = 0x200,    //用例说明
    ES_FS_CHILD     = 0x400,    //从属的二级科目
    ES_FS_DEFSUB    = 0x800     //默认二级科目
};
Q_DECLARE_FLAGS(FirstSubjectEditStates, FirstSubjectEditState)
Q_DECLARE_OPERATORS_FOR_FLAGS(FirstSubjectEditStates)

//二级科目编辑状态
enum SecondSubjectEditState{
    ES_SS_INIT      = 0x0,  //未做任何修改
    ES_SS_FID       = 0x01, //父科目
    ES_SS_NID       = 0x02, //科目所用的名称条目id
    ES_SS_CODE      = 0x04, //科目代码
    ES_SS_WEIGHT    = 0x08, //权重
    ES_SS_ISENABLED = 0x10, //是否启用
    ES_SS_DISABLE   = 0x20, //禁用时间
    ES_SS_CTIME     = 0x40, //创建时间
    ES_SS_CUSER     = 0x80  //创建者
};
Q_DECLARE_FLAGS(SecondSubjectEditStates, SecondSubjectEditState)
Q_DECLARE_OPERATORS_FOR_FLAGS(SecondSubjectEditStates)

//金额方向枚举
enum MoneyDirection{
    MDIR_J  =   1,
    MDIR_P  =   0,
    MDIR_D  =   -1
};
Q_DECLARE_METATYPE(MoneyDirection)

//凭证状态代码
enum PzState{
    Pzs_NULL       =    -1,   //空状态
    Pzs_Repeal     =    0,    //作废
    Pzs_Recording  =    1,    //初始录入态
    Pzs_Verify     =    2,    //已审核
    Pzs_Instat     =    3,    //已入账（可计入统计）
    Pzs_Max        =    100   //这个数是一个标志，它指示该凭证是刚插入到数据库的，还没有回读它的id

};
Q_DECLARE_METATYPE(PzState)

//凭证类别代码
enum PzClass{
    Pzc_NULL      = -1,        //空凭证
    Pzc_Hand      =   0,       //手工录入的凭证（由用户添加，并允许用户修改）

    //由其他模块引入的凭证（由其他模块添加，允许人工修改）
    Pzc_GdzcImp   =   11,      //引入固定资产的凭证
    Pzc_GdzcZj    =   12,      //固定资产折旧凭证
    Pzc_DtfyImp   =   14,      //引入待摊费用的凭证
    Pzc_DtfyTx    =   15,      //待摊费用分摊凭证
                               //工资
                               //计提税金

    //自动结转凭证（由系统添加，且不允许人工修改）
    Pzc_Jzhd      =   30,      //这是新的表示结转汇兑损益凭证的类别代码
    Pzc_Jzhd_Bank =   31,      //结转汇兑损益-银行存款
    Pzc_Jzhd_Ys   =   32,      //结转汇兑损益-应收账款
    Pzc_Jzhd_Yf   =   33,      //结转汇兑损益-应付账款
    Pzc_JzsyIn    =   34,      //结转损益（收入类）
    Pzc_JzsyFei   =   35,      //结转损益（费用类）
    Pzc_Jzhd_Yus  =   36,      //结转汇兑损益-预收账款
    Pzc_Jzhd_Yuf  =   37,      //结转汇兑损益-预付账款

    //其他需由系统添加，并允许人工修改的凭证
    Pzc_Jzlr      =   50       //结转本年利润到利润分配
};
Q_DECLARE_METATYPE(PzClass)

/**
 * @brief 一级科目大类类别枚举
 */
enum SubjectClass{
    SC_NULL = -1,       //不属于任何类别
    SC_ALL  = 0,        //所有
    SC_ZC   = 1,        //资财类
    SC_FZ   = 2,        //负债类
    SC_QY   = 3,        //所有者权益类
    SC_CB   = 4,        //成本类
    SC_SY   = 5,        //损益类
    SC_GT   = 6         //共同类
};

////凭证子类别
//enum PzSubClass{
//    Pzc_sub_jzhd_bank   = 1,    //结转银行存款
//    Pzc_sub_jzhd_ys     = 2,    //结转应收账款
//    Pzc_sub_jzhd_yf     = 3,    //结转应付账款
//    Pzc_sub_jzhd_yus    = 4,    //结转预收账款
//    Pzc_sub_jzhd_yuf    = 5     //结转预付账款
//};

class SubjectNameItem;
class SecondSubject;
struct Bank;

struct BankAccount{
    int id;
    Bank* parent;
    Money* mt;              //该账户所对应的币种
    QString accNumber;      //帐号
    SubjectNameItem* niObj; //对应的名称条目对象（这个域在Account对象初始化阶段就要设置）
    //SecondSubject* subObj;  //对应的二级科目对象（这个域只在科目管理器返回此结构时设置）
};

struct Bank{
    int id;
    bool isMain;                //是否基本户
    QString name,lname;         //简称和全称
    QList<BankAccount*> bas;    //该开户行下开设的账户
};

//保存日记账表格行数据的结构
//金额式表格行
struct RowTypeForJe{
    int y,m,d;       //凭证日期
    QString pzNum;   //凭证号码
    QString summary; //凭证摘要
    int dh;         //本次发生方向
    double v;        //本次发生金额
    double em;       //余额
    int dir;         //余额借贷方向
    int pid;         //该业务活动所属的凭证id
    int sid;         //该业务活动的id（BusiActions表id字段）
};


//外币金额式表格行
struct RowTypeForWj{
    int y,m,d;       //凭证日期
    QString pzNum;   //凭证号码
    QString summary; //凭证摘要
    int dh;          //本次发生方向
    int mt;          //本次发生的币种代码
    double v;        //本次发生金额
    QHash<int,double> em; //分币种余额(key为币种代码)
    double etm;      //各币种合计余额值
    int dir;         //总余额借贷方向
    int pid;         //该业务活动所属的凭证id
    int bid;         //该业务活动的id（BusiActions表id字段）
};

//日记账/明细账单行数据结构
struct DailyAccountData{
    int y,m,d;       //凭证日期
    QString pzNum;   //凭证号码
    QString summary; //凭证摘要
    QString jsNum;   //结算号
    int     oppoSub; //对方科目
    int dh;          //本次发生方向
    int mt;          //本次发生的币种代码
    double v;        //本次发生金额
    int dir;         //总余额借贷方向
    int pid;         //该业务活动所属的凭证id
    int bid;         //该业务活动的id（BusiActions表id字段）
    //这些余额表示，到本次业务活动发生后，该科目的当前余额
    QHash<int,double> em; //按币种分开核算的余额(key为币种代码)
    QHash<int,int> dirs;  //各币种的余额方向
    double etm;           //各币种混合核算的余额值
};

struct DailyAccountData2{
    int y,m,d;       //凭证日期
    QString pzNum;   //凭证号码（有中文字符，因此用QString）
    QString summary; //凭证摘要
    QString jsNum;   //结算号
    int     oppoSub; //对方科目
    int dh;          //本次发生方向
    int mt;          //本次发生的币种代码
    Double v;        //本次发生金额
    int dir;         //总余额借贷方向
    int pid;         //该业务活动所属的凭证id
    int bid;         //该业务活动的id（BusiActions表id字段）
    //这些余额表示，到本次业务活动发生后，该科目的当前余额
    QHash<int,Double> em; //按币种分开核算的余额(key为币种代码)
    QHash<int,int> dirs;  //各币种的余额方向
    Double etm;           //各币种混合核算的余额值
};

//明细账视图搜寻条件结构：
struct DVFilterRecord{
    CommonItemEditState editState;  //
    int id;
    int suiteId;                    //帐套id
    bool isDef;                     //是否是系统默认的过滤条件
    bool isCur;                     //是否是最后关闭窗口时应用的过滤条件
    bool isFst;                     //科目范围是一级科目还是二级科目
    int curFSub;                    //当前一级科目
    int curSSub;                    //当前二级科目
    int curMt;                      //当前币种代码
    QString name;                   //名称
    QDate startDate,endDate;        //开始时间、结束时间    
    QList<int> subIds;              //选定的科目代码列表（如果选定了一个一级科目，则首元素保存此一级科目的id，
                                    //后续跟随二级科目，否者此列表保存所有选择的一级科目id）
};
Q_DECLARE_METATYPE(DVFilterRecord*)

//总账单行数据结构
struct TotalAccountData{
    int y,m;  //帐套年份、月份
    QHash<int,double> jvh; //本月借方合计（键均为币种代码）
    double jv;             //本月借方合计（用母币计）
    QHash<int,double> dvh; //本月贷方合计金额
    double dv;             //本月贷方合计（用母币计）    
    QHash<int,double> evh; //余额
    double ev;             //余额（用母币计）
    QHash<int,int> dirs;   //余额方向
    int dir;               //余额方向（用母币计）

};

struct TotalAccountData2{
    int y,m;  //帐套年份、月份
    QHash<int,Double> jvh; //本月借方合计（键均为币种代码）
    Double jv;             //本月借方合计（用母币计）
    QHash<int,Double> dvh; //本月贷方合计金额
    Double dv;             //本月贷方合计（用母币计）
    QHash<int,Double> evh; //余额
    Double ev;             //余额（用母币计）
    QHash<int,int> dirs;   //余额方向
    int dir;               //余额方向（用母币计）

};

/**
 * @brief The PzPrintData struct
 *  打印一页凭证需要的数据结构（如果一个凭证对象包含的分录对象超出了一页凭证可拥有的最大行数，则用多个此对象表示属于同一张凭证的凭证数据）
 */
struct PzPrintData{
    QDate date;           //凭证日期
    int attNums;          //附件数
    QString pzNum;        //凭证号
    QString pzZbNum;      //自编号
    QList<BusiAction*> baLst; //凭证业务活动列表
    Double jsum,dsum;     //借贷合计值
    User* producer;     //制单者
    User* verify;       //审核者
    User* bookKeeper;   //记账者
};

//包含业务活动数据的结构
struct BusiActionData{
    enum ActionState{
        DELETED = -1,  //被删除的
        BLANK   = 0,   //空白行
        INIT = 1,      //初始读入的
        EDITED = 2,    //被修改了
        NEW = 3,       //新增的
        NUMCHANGED = 4 //序号改变了
    };

    BusiActionData(){}
    BusiActionData(BusiActionData* other){
        id = other->id;
        pid = other->pid;
        summary = other->summary;
        fid = other->fid;
        sid = other->sid;
        mt = other->mt;
        dir = other->dir;
        v = other->v;
        num = other->num;
        state = NEW;
    }

    int id;           //该业务活动的
    int pid;          //业务活动所属凭证id
    QString summary;  //摘要
    int fid;          //一级科目id
    int sid;          //二级科目id
    int mt;           //币种
    int dir;          //借贷方向（1：借，0：贷，-1：未定）
    double v;         //金额
    int num;          //该业务活动在凭证中的序号
    ActionState state;//业务活动状态
};

//包含业务活动数据的结构(用Double类代替double数据类型)
struct BusiActionData2{
    enum ActionState{
        DELETED = -1,  //被删除的
        BLANK   = 0,   //空白行
        INIT = 1,      //初始读入的
        EDITED = 2,    //被修改了
        NEW = 3,       //新增的
        NUMCHANGED = 4 //序号改变了
    };

    BusiActionData2(){}
    BusiActionData2(BusiActionData2* other){
        id = other->id;
        pid = other->pid;
        summary = other->summary;
        fid = other->fid;
        sid = other->sid;
        mt = other->mt;
        dir = other->dir;
        v = other->v;
        num = other->num;
        state = NEW;
    }

    int id;           //该业务活动的
    int pid;          //业务活动所属凭证id
    QString summary;  //摘要
    int fid;          //一级科目id
    int sid;          //二级科目id
    int mt;           //币种
    int dir;          //借贷方向（1：借，0：贷，-1：未定）
    Double v;         //金额
    int num;          //该业务活动在凭证中的序号
    ActionState state;//业务活动状态
};

enum PzEditState{
    PZDELETEED    = -1,   //凭证被删除了
    PZINIT        = 0,    //凭证刚从数据库中读取，或已经执行了保存
    PZINFOEDITED  = 1,    //凭证信息内容被编辑了（这些内容的改变不会影响统计结果）
    PZVALUEEDITED = 2,    //凭证的金额发生了改变
    PZNEW         = 3     //凭证是新的，数据表中还没有对应记录，需要执行一次插入操作
};

struct PzData{
    int pzId;                     //凭证id
    QString date;                 //凭证日期
    int attNums;                  //附件数
    int pzNum;                    //凭证号
    int pzZbNum;                  //自编号
    QList<BusiActionData*> baLst; //凭证业务活动列表
    double jsum,dsum;             //借贷合计值
    PzEditState editState;        //凭证编辑状态
    PzState state;                //凭证状态
    PzClass pzClass;              //凭证类别
    User* producer;               //制单者
    User* verify;                 //审核者
    User* bookKeeper;             //记账者
};

struct PzData2{
    int pzId;                     //凭证id
    QString date;                 //凭证日期
    int attNums;                  //附件数
    int pzNum;                    //凭证号
    int pzZbNum;                  //自编号
    QList<BusiActionData2*> baLst; //凭证业务活动列表
    Double jsum,dsum;             //借贷合计值
    PzEditState editState;        //凭证编辑状态
    PzState state;                //凭证状态
    PzClass pzClass;              //凭证类别
    User* producer;               //制单者
    User* verify;                 //审核者
    User* bookKeeper;             //记账者
};

//MDI子窗口位置和尺寸信息
struct SubWindowDim{
    int x,y;  //子窗口位置
    int w,h;  //子窗口大小
};

//账户转移状态枚举类型（待引入账户的转入转出功能后，移到transform.h文件中）
enum AccountTransferState{
    ATS_INVALID     =   0,      //无效状态
    ATS_TRANSOUTED  =   1,      //已转出
    ATS_TRANSINDES  =   2,      //已转入目标主机
    ATS_TRANSINOTHER =  3       //已转入其他主机
};

//主机类型
enum MachineType{
   MT_COMPUTER  = 1,    //物理电脑
   MT_CLOUDY    = 2     //云账户
};

//账户简要信息，此信息来自于基本数据库的AccountInfos表
//struct AccountBriefInfo{
//    int id;                     //账户id
//    QString code;               //账户代码
//    QString fname;              //账户数据库文件名
//    QString sname;              //账户简称
//    QString lname;              //账户全称
//    bool isRecent;              //是否是最近打开的账户
//    AccountTransferState tstate;//转移状态（待转移功能加入后使用）
//    QDateTime tinTime;          //转入时间
//    Machine* outMachine;        //转出主机
//    QDateTime tOutTime;         //转出时间
//    QString hashValue;          //账户文件的Hash值
//};



/**
 * @brief 账户缓存条目结构（与本地账户缓存表对应）
 */
struct AccountCacheItem{
    AccountCacheItem(){}
    AccountCacheItem(AccountCacheItem& other){
        code = other.code;
        fileName = other.fileName;
        accName = other.accName;
        accLName = other.accLName;
        inTime = other.inTime;
        outTime = other.outTime;
        mac = other.mac;
        tState = other.tState;
        lastOpened = other.lastOpened;
    }

    int id;
    QString code;       //账户代码
    QString fileName;   //账户数据库文件名
    QString accName;    //账户简称
    QString accLName;   //账户全称
    QDateTime inTime;   //转入时间（最近一次转入账户到本主机的时间，三种转移状态下都有意义）
    QDateTime outTime;  //转出时间（已转出时有意义，其他状态无意义）
    Machine* mac;    //要转入的目的主机（已转出），转出此账户的源主机（转入到目的机或其他机）
    AccountTransferState tState; //转移状态
    bool lastOpened;    //是否是最后打开的账户
};

/**
 * @brief 科目系统名称条目
 */
struct SubSysNameItem{
    int code;               //科目系统代码
    QString name,explain;   //科目系统名称及其解释
    bool isImport;          //是否已导入给科目系统的科目
};

/**
 * @brief 科目系统衔接配置条目结构
 */
struct SubSysJoinItem{
    FirstSubject *sFSub, *dFSub; //源一级科目，目的一级科目
    bool isMap;                 //是否建立衔接映射
    QList<int> ssubMaps;        //子目映射列表（每两个元素为一组，前一个是源子目id，后一个是目的子目id）
};

/**
 * @brief 帐套记录结构
 */
struct AccountSuiteRecord{
    int id;
    int year,recentMonth;       //帐套所属年份、最后打开月份
    int startMonth,endMonth;    //开始月份和结束月份
    int subSys;                 //帐套采用的科目系统代码
    QString name;               //帐套名
    bool isClosed;              //是否已关账
    bool isUsed;                //帐套是否已启用
    bool isCur;                 //是否当前帐套

    bool operator !=(const AccountSuiteRecord& other);
};

//凭证错误级别
enum PingZhengErrorLevel{
    PZE_WARNING   = 1,       //警告级
    PZE_ERROR     = 2        //错误级
};

//凭证错误信息（由凭证集检错方法使用）
struct PingZhengError{
    PingZhengErrorLevel errorLevel;      //错误级别
    int errorType;       //错误类型
    PingZheng* pz;       //出现错误的凭证对象
    BusiAction* ba;      //出现错误的分录对象
    QString extraInfo;   //额外补充信息
    QString explain;     //描述错误的信息
};

//可以在MDI区域打开的子窗口类型代码
enum subWindowType{
    SUBWIN_NONE       = 0,    //不指代任何子窗口类型
    SUBWIN_PZEDIT     = 1,    //凭证编辑窗口
    SUBWIN_PZSTAT     = 2,    //本期统计窗口
    SUBWIN_PZSTAT2    = 3,    //本期统计窗口（新）
    SUBWIN_DETAILSVIEW2 = 4,  //明细账视图（新）
    SUBWIN_PZEDIT_new     = 5,    //凭证编辑窗口（新）
    //CASHDAILY  = 3,    //现金日记账窗口
    //BANKDAILY  = 4,    //银行日记账窗口
    //DETAILSDAILY=5,    //明细科目日记账窗口
    TOTALDAILY = 6,    //总分类账窗口
    SUBWIN_SETUPBASE  = 7,    //设置账户期初余额窗口
    //SETUPBANK  = 8,    //设置开户行信息
    SUBWIN_BASEDATAEDIT = 9,  //基本数据库编辑窗口
    SUBWIN_GDZCADMIN =  10,   //固定资产管理窗口
    //DTFYADMIN = 11,    //待摊费用管理窗口
    TOTALVIEW = 12,    //总账视图
    DETAILSVIEW = 13,  //明细账视图
    SUBWIN_HISTORYVIEW = 14,  //历史凭证
    SUBWIN_LOOKUPSUBEXTRA =15,//查看科目余额
    SUBWIN_ACCOUNTPROPERTY=16,//查看账户属性
    SUBWIN_VIEWPZSETERROR=17,  //查看凭证错误窗口
    SUBWIN_SQL = 18           //SQL工具窗口
    //设置期初余额的窗口
    //科目配置窗口
};

//enum MachineType{
//    MT_COMPUTER = 1,    //电脑
//    MT_CLOUDY   = 2     //云
//};

///**
// * @brief The Machine struct
// *  可以放置账户文件以只读或编辑模式打开的机器
// */

//struct Machine{
//    int id;             //主机id
//    MachineType type;   //主机类型
//    bool isLocal;       //是否本机
//    QString name;       //主机名
//    QString desc;       //主机描述
//};

#endif // COMMDATASTRUCT_H
