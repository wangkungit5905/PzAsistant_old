
#include "subject.h"
#include "logs/Logger.h"
#include "dbutil.h"
#include "global.h"

QHash<int,QStringList> SubjectManager::nameItemCls;
QHash<int,SubjectNameItem*> SubjectManager::nameItems;
QList<SubjectNameItem*> SubjectManager::delNameItems;

//虚拟科目对象
//FirstSubject* SubjectManager::FS_ALL = new FirstSubject(0,);
//FirstSubject* SubjectManager::FS_NULL;
//SubjectNameItem* NI_ALL;
//SubjectNameItem* NI_NULL;
//SecondSubject* SS_ALL;
//SecondSubject* SS_NULL;


/////////////////////////////////FirstSubject///////////////////////////

FirstSubject::FirstSubject(const FirstSubject &other)
{
    _parent = other._parent;
    md = FSTSUBMD++;
    id = other.id;  //应该另外创建id？
    name = other.name;
    code = other.code;
    weight = other.weight;
    jdDir = other.jdDir;
    isUseWb = other.isUseWb;
    editState = other.editState;
    remCode = other.remCode;
    subClass = other.subClass;
    briefExplain = other.briefExplain;
    usage = other.usage;
    subSys = other.subSys;
    childSubs = other.childSubs;
    defSub = other.defSub;
}

FirstSubject::FirstSubject(SubjectManager* parent, int id, SubjectClass subcls, QString subName, QString subCode, QString remCode,
            int subWeight,bool isEnable,bool jdDir, bool isUseWb, QString explain, QString usage, int subSys):
    SubjectBase(),_parent(parent),md(FSTSUBMD++),id(id),subClass(subcls),name(subName),code(subCode),remCode(remCode),
    weight(subWeight),isEnable(isEnable),jdDir(jdDir),isUseWb(isUseWb),
    briefExplain(explain),usage(usage),subSys(subSys),defSub(NULL)
{
}

FirstSubject::~FirstSubject()
{
    qDeleteAll(childSubs);
}

void FirstSubject::setCode(QString subCode)
{
    QString c = subCode.trimmed();
    if(code != c){
        code = c;
        witchEdited|=ES_FS_CODE;
    }
}

void FirstSubject::setWeight(int subWeight)
{
    if(subWeight!= weight)
    {
        weight = subWeight;
        witchEdited|=ES_FS_WEIGHT;
    }
}

void FirstSubject::setEnabled(bool en)
{
    if(!en && isEnable || en && !isEnable)
    {
        isEnable = en;
        witchEdited |= ES_FS_ISENABLED;
    }
}

void FirstSubject::setName(QString subName)
{
    QString n = subName.trimmed();
    if(n!=name){
        name = subName;
        witchEdited |= ES_FS_NAME;
    }
}

void FirstSubject::setRemCode(QString code)
{
    QString c = code.trimmed();
    if(c != remCode){
       remCode=code;
       witchEdited |= ES_FS_SYMBOL;
    }
}

void FirstSubject::setJdDir(bool dir)
{
    if(!dir && jdDir || dir && !jdDir){
       jdDir=dir;
       witchEdited |= ES_FS_JDDIR;
    }
}

void FirstSubject::setIsUseForeignMoney(bool isUse)
{
    if(isUseWb ^ isUse){
        isUseWb = isUse;
        witchEdited |= ES_FS_ISUSEWB;
    }
}

void FirstSubject::setSubClass(SubjectClass c)
{
    if(subClass != c){
        subClass=c;
        witchEdited = ES_FS_CLASS;
    }
}

void FirstSubject::setBriefExplain(QString s)
{
    QString desc = s.trimmed();
    if(desc != briefExplain){
        briefExplain = desc;
        witchEdited |= ES_FS_DESC;
    }
}

void FirstSubject::setUsage(QString s)
{
    QString u = s.trimmed();
    if(usage != u){
        usage = u;
        witchEdited |= ES_FS_USAGE;
    }
}

/**
 * @brief FirstSubject::getAllSSubIds
 *  返回所有属于该一级科目的二级科目的id列表
 * @return
 */
QList<int> FirstSubject::getAllSSubIds()
{
    QList<int> ids;
    foreach(SecondSubject* ssub, childSubs)
        ids<<ssub->getId();
    return ids;
}

/**
 * @brief FirstSubject::addChildSub
 * @param sub
 * @return
 */
void FirstSubject::addChildSub(SecondSubject *sub)
{
    if(!sub)
        return;
    if(childSubs.contains(sub))
        return;
    sub->setParent(this);
    childSubs<<sub;
    witchEdited |= ES_FS_CHILD;
}

/**
 * @brief FirstSubject::addChildSub
 *  使用指定的名称条目对象创建二级科目
 * @param name
 * @param Code
 * @param subWeight
 * @param isEnable
 * @param isInit
 * @return
 */
SecondSubject *FirstSubject::addChildSub(SubjectNameItem *ni, QString Code, int subWeight, bool isEnable)
{
    if(!ni)
        return NULL;
    foreach(SecondSubject* sub, childSubs)
        if(sub->getNameItem() == ni)
            return NULL;
    SecondSubject* sb = new SecondSubject(this,0,ni,Code,subWeight,isEnable,QDateTime::currentDateTime(),
                                          QDateTime::currentDateTime(),curUser);
    childSubs<<sb;
    witchEdited |= ES_FS_CHILD;
    return sb;
}

/**
 * @brief FirstSubject::addChildSub
 *  利用已有的名称条目添加新子目
 * @param name      子目所使用的名称条目对象
 * @param Code      子目的代码
 * @param subWeight 子目权重
 * @param isEnable  子目是否启用
 * @param isInit    是否在科目管理器的初始化阶段调用此方法
 * @return
 */
//SecondSubject *FirstSubject::addChildSub(SecondSubject *ssub)
//{
//    if(!ssub){
//        LOG_ERROR("SecondSubject object is NULL!");
//        return NULL;
//    }
//    foreach(SecondSubject* sub, childSubs){
//        if(sub->getNameItem() == ssub){
//            LOG_ERROR(QObject::tr("SecondSubject(%1) have existed in FirstSubject(%2)")
//                      .arg(sub->getName()).arg(name));
//            return NULL;
//        }
//    }
//    SecondSubject* ssub = new SecondSubject(this,0,ni,code,weight,isEnabled,crtTime,creator);
//    if(!dbUtil->saveSndSubject(ssub)){
//        delete ssub;
//        return NULL;
//    }

////    SecondSubject* sb = new SecondSubject(this,0,ssub,Code,subWeight,isEnable,crtTime,QDateTime(),curUser);
//    childSubs<<ssub;
//    witchEdited |= ES_FS_CHILD;
//    return sb;
//}

//void FirstSubject::addChildSub(SecondSubject *sub, bool isInit)
//{
//    if(sub){
//        childSubs<<sub;
//        sub->setParent(this);
//        if(!isInit)
//            witchEdited |= ES_FS_CHILD;
//    }
//}

/**
 * @brief FirstSubject::restoreChildSub
 *  恢复被删除的子目对象（被删除的子目，如果为执行保存操作，则仍保留在删除队列中）
 * @param sub
 * @return
 */
SecondSubject *FirstSubject::restoreChildSub(SecondSubject *sub)
{
    for(int i = 0; i< delSubs.count(); ++i){
        if(delSubs.at(i) == sub){
            sub->setDelete(false);
            childSubs<<delSubs.takeAt(i);
            witchEdited |= ES_FS_CHILD;
            return childSubs.last();
        }
    }
    return NULL;
}

/**
 * @brief FirstSubject::restoreChildSub
 *  恢复被移除的使用指定名称对象的二级科目
 * @param ni
 * @return
 */
SecondSubject *FirstSubject::restoreChildSub(SubjectNameItem *ni)
{
    for(int i = 0; i< delSubs.count(); ++i){
        if(delSubs.at(i)->getNameItem() == ni){
            delSubs.at(i)->setDelete(false);
            childSubs<<delSubs.takeAt(i);
            witchEdited |= ES_FS_CHILD;
            return childSubs.last();
        }
    }
    return NULL;
}

/**
 * @brief FirstSubject::removeChildSub
 *  移除子目
 * @param sub
 * @return
 */
bool FirstSubject::removeChildSub(SecondSubject *sub)
{
    if(!sub)
        return false;
    if(!childSubs.removeOne(sub))
        return false;
    delSubs<<sub;
    sub->setDelete(true);
    witchEdited |= ES_FS_CHILD;
    return true;
}

SecondSubject *FirstSubject::getChildSub(int index)
{
    if(index<0 || index>=childSubs.count())
        return 0;
    else
        return childSubs.at(index);
}

/**
 * @brief FirstSubject::getChildSubIds
 *  获取所有子科目的id列表
 * @return
 */
QList<int> FirstSubject::getChildSubIds()
{
    QList<int> ids;
    for(int i = 0; i < childSubs.count(); ++i)
        ids<<childSubs.at(i)->getId();
    return ids;
}

/**
 * @brief FirstSubject::getRangeChildSubs
 *  返回指定范围的子目对象
 * @param ssub：起始科目
 * @param esub：终止科目
 * @param subs：在范围内的科目
 * @return
 */
bool FirstSubject::getRangeChildSubs(SecondSubject *ssub, SecondSubject *esub, QList<SecondSubject *> &subs)
{
    if(childSubs.empty())
        return true;
    SecondSubject* sub = childSubs.first();
    int count = childSubs.count();
    int si;
    if(!ssub)
        si = 0;
    else{
        si = 1;
        while(sub != ssub && si < count)
            sub = childSubs.at(si++);
    }

    int ei;
    if(!esub)
        ei = count-1;
    else{
        ei = si;
        while(sub != esub && si < count)
            sub = childSubs.at(ei++);
    }
    if(si == count || ei == count){
        LOG_ERROR("Index overflow in function FirstSubject::getRangeChildSubs()");
        return false;
    }
    for(int i = si; i <= ei; i++)
        subs<<childSubs.at(i);
    return true;
}

/**
 * @brief FirstSubject::containChildSub
 *  是否包含指定的子目
 * @param ni
 * @return
 */
bool FirstSubject::containChildSub(SecondSubject *sndSub)
{
    if(!sndSub)
        return false;
    return childSubs.contains(sndSub);
}

/**
 * @brief FirstSubject::containChildSub
 *  是否包含使用了指定名称条目的子目对象
 * @param ni
 * @return
 */
bool FirstSubject::containChildSub(SubjectNameItem *ni)
{
    if(!ni)
        return false;
    for(int i = 0; i < childSubs.count(); ++i)
        if(ni == childSubs.at(i)->getNameItem())
            return true;
    return false;
}

/**
 * @brief FirstSubject::getChildSub
 *  返回该一级科目下使用了指定名称条目的二级科目对象
 * @param ni
 * @return
 */
SecondSubject *FirstSubject::getChildSub(SubjectNameItem *ni)
{
    for(int i = 0; i < childSubs.count(); ++i){
        if(ni == childSubs.at(i)->getNameItem())
            return childSubs.at(i);
    }
    return NULL;
}

/**
 * @brief FirstSubject::setDefaultSubject
 *  设置默认的二级科目
 * @param ssub
 */
void FirstSubject::setDefaultSubject(SecondSubject *ssub)
{
    if(defSub == ssub)
        return;
    if(defSub)
        defSub->setWeight(INIT_WEIGHT);
    defSub = ssub;
    defSub->setWeight(DEFALUT_SUB_WEIGHT);
    witchEdited |= ES_FS_DEFSUB;
}


//针对一级科目的排序比较函数
bool byNameThan_fs(FirstSubject *fs1, FirstSubject *fs2)
{return fs1->getName() < fs2->getName();}
bool bySubCodeThan_fs(FirstSubject *fs1, FirstSubject *fs2)
{return fs1->getCode() < fs2->getCode();}
bool byRemCodeThan_fs(FirstSubject *fs1, FirstSubject *fs2)
{return fs1->getRemCode() < fs2->getRemCode();}

//针对名称条目的排序比较函数
bool byRemCodeThan_ni(SubjectNameItem *ni1, SubjectNameItem *ni2)
{return ni1->getRemCode() < ni2->getRemCode();}
bool byNameThan_ni(SubjectNameItem *ni1, SubjectNameItem *ni2)
{return ni1->getShortName() < ni2->getShortName();}

//针对二级科目的排序比较函数
bool bySubCodeThan_ss(SecondSubject *ss1, SecondSubject *ss2)
{return ss1->getCode() < ss2->getCode();}
bool byRemCodeThan_ss(SecondSubject *ss1, SecondSubject *ss2)
{return ss1->getRemCode() < ss2->getRemCode();}
bool bySubNameThan_ss(SecondSubject *ss1, SecondSubject *ss2)
{return ss1->getName() < ss2->getName();}



//////////////////////////FSubItrator/////////////////////////////////////////////
FSubItrator::FSubItrator(QHash<int, FirstSubject *> &fsubs):fsubHash(fsubs)
{
    QList<FirstSubject*> fsubObjs = fsubHash.values();
    qSort(fsubObjs.begin(),fsubObjs.end(),bySubCodeThan_fs);
    for(int i = 0; i < fsubObjs.count(); ++i)
        ids<<fsubObjs.at(i)->getId();
    index = -1;
}

bool FSubItrator::hasNext()
{
    return (index < ids.count()-1);
}

FirstSubject *FSubItrator::next()
{
    return fsubHash.value(ids.at(++index));
}

int FSubItrator::key()
{
    if(index > ids.count() - 1)
        return 0;
    else
        return ids.at(index);
}

FirstSubject *FSubItrator::value()
{
    if(index > ids.count() - 1)
        return NULL;
    else
        return fsubHash.value(ids.at(index));
}

void FSubItrator::toFront()
{
    index = -1;
}


//////////////////////////SubjectNameItem//////////////////////////////////////////
/**
 * @brief SubjectNameItem::SubjectNameItem
 *  此构造函数用于初始化名称条目期间
 * @param id
 * @param cls
 * @param sname
 * @param lname
 * @param remCode
 * @param crtTime
 * @param user
 */
SubjectNameItem::SubjectNameItem(int id,int cls,QString sname,QString lname,QString remCode,
                                 QDateTime crtTime,User* user):
    id(id),clsId(cls),sname(sname),lname(lname),remCode(remCode),crtTime(crtTime),
    crtUser(curUser),isDeleted(false)
{
    md = NAMEITEMMD++;

}

/**
 * @brief SubjectNameItem::SubjectNameItem
 *  在动态生成名称条目时调用
 */
SubjectNameItem::SubjectNameItem():
    id(UNID),clsId(UNCLASS),isDeleted(false),crtTime(QDateTime::currentDateTime()),
        crtUser(curUser)
{
    md=NAMEITEMMD++;
    witchEdit = ES_NI_INIT;
    witchEdit |= ES_NI_CLASS;
}


SubjectNameItem::SubjectNameItem(const SubjectNameItem &other)
{
    md = NAMEITEMMD++;
    witchEdit = other.witchEdit;
    id = other.id;
    clsId = other.clsId;
    sname = other.sname;
    lname = other.lname;
    remCode = other.remCode;
    crtTime = other.crtTime;
    crtUser = other.crtUser;
    isDeleted = other.isDeleted;
}

void SubjectNameItem::setClassId(int cid)
{
    if(cid != clsId){
        clsId = cid;
        witchEdit |= ES_NI_CLASS;
    }
}

void SubjectNameItem::setShortName(QString name)
{
    QString n = name.trimmed();
    if(sname != n){
        sname = n;
        witchEdit |= ES_NI_SNAME;
    }
}

void SubjectNameItem::setLongName(QString name)
{
    QString n = name.trimmed();
    if(lname != n){
        lname = n;
        witchEdit |= ES_NI_LNAME;
    }
}

void SubjectNameItem::setRemCode(QString code)
{
    QString c = code.trimmed();
    if(remCode != c){
        remCode = c;
        witchEdit |= ES_NI_SYMBOL;
    }
}



/////////////////////////////SecondSubject///////////////////////////////////////////////
/**
 * @brief SecondSubject::SecondSubject
 *  用于在初始化期间、利用已有名称条目在指定一级科目下创建二级科目的时候
 * @param p
 * @param id
 * @param name
 * @param Code
 * @param subWeight
 * @param isEnable
 * @param crtTime
 * @param disTime
 * @param crtUser
 */
SecondSubject::SecondSubject(FirstSubject *p, int id, SubjectNameItem *name, QString Code, int subWeight,
                             bool isEnable, QDateTime crtTime, QDateTime disTime, User *crtUser):
    parent(p),id(id),nItem(name),code(Code),weight(subWeight),isEnable(isEnable),isDeleted(false),
    crtTime(crtTime),disTime(disTime),creator(crtUser)
{
    md=SNDSUBMD++;
}


SecondSubject::SecondSubject(const SecondSubject& other)
{
    md=SNDSUBMD++;
    //id = other.id;
    id = UNID;
    code = other.code;
    weight = other.weight;
    isEnable = other.isEnable;
    parent = other.parent;
    nItem = other.nItem;
    crtTime = other.crtTime;
    disTime = other.disTime;
    creator = other.creator;
    isDeleted = other.isDeleted;
}

/**
 * @brief SecondSubject::SecondSubject
 *  创建新的二级科目对象（在还没有创建名称信息条目对象的情况下调用）
 * @param p
 * @param id
 * @param nameId
 * @param nameClsId
 * @param sName
 * @param lName
 * @param subCode
 * @param remCode
 * @param subWeight
 * @param isEnable
 * @param crtUser
 * @param crtTime
 */
SecondSubject::SecondSubject(FirstSubject* p, int id, int nameId, int nameClsId, QString sName, QString lName, QString subCode, QString remCode,
                             int subWeight, bool isEnable, User *crtUser, QDateTime crtTime):
    parent(p),id(id),isDeleted(false)
{
    //首先要检查给定的名称是否存在，如存在，则使用现存的名称条目的其他信息，如不存在，则创建新的名称条目并使用之
    //待以后实现
    md=SNDSUBMD++;
    nItem = new SubjectNameItem;
    nItem->setClassId(nameClsId);
    nItem->setShortName(sName);
    nItem->setLongName(lName);
    nItem->setRemCode(remCode);
    //nItem
}

SecondSubject::SecondSubject():
    id(UNID),crtTime(QDateTime::currentDateTime()),creator(curUser),isDeleted(false)
{
    md=SNDSUBMD++;
}

void SecondSubject::setCode(QString subCode)
{
    QString c =subCode.trimmed();
    if(c != code){
        code = c;
        witchEdit |= ES_SS_CODE;
        parent->childEdited();
    }
}

void SecondSubject::setWeight(int subWeight)
{
    if(weight != subWeight){
        weight = subWeight;
        witchEdit |= ES_SS_WEIGHT;
        parent->childEdited();
    }
}

void SecondSubject::setEnabled(bool en)
{
    if(en && !isEnable || !en && isEnable){
        isEnable=en;
        witchEdit |= ES_SS_ISENABLED;
        parent->childEdited();
    }
}

/**
 * @brief SecondSubject::isDef
 * @return
 */
bool SecondSubject::isDef()
{
    if(!parent)
        return false;
    if(parent->getDefaultSubject() == this)
        return true;
    else
        return false;
}

////////////////////////////SubjectManager////////////////////////////////////////////
/**
 * @brief SubjectManager::getNameItem
 *  返回名称条目的简称与name相等的名称对象
 * @param name
 * @return
 */
SubjectNameItem *SubjectManager::getNameItem(QString name)
{
    QHashIterator<int,SubjectNameItem*> it(nameItems);
    while(it.hasNext()){
        it.next();
        if(name == it.value()->getShortName())
            return it.value();
    }
    return NULL;
}

bool SubjectManager::containNI(QString name)
{
    QHashIterator<int,SubjectNameItem*> it(nameItems);
    while(it.hasNext()){
        it.next();
        if(name == it.value()->getShortName())
            return true;
    }
    return false;
}

bool SubjectManager::containNI(SubjectNameItem *ni)
{
    QHashIterator<int,SubjectNameItem*> it(nameItems);
    while(it.hasNext()){
        it.next();
        if(ni == it.value())
            return true;
    }
    return false;
}

/**
 * @brief SubjectManager::getUseWbSubs
 *  获取所有使用外币的科目
 * @param
 * @return
 */
void SubjectManager::getUseWbSubs(QList<FirstSubject*>& fsubs)
{
    foreach(FirstSubject* fsub, fstSubHash.values()){
        if(fsub->isUseForeignMoney())
            fsubs<<fsub;
    }
}

/**
 * @brief SubjectManager::getSyClsSubs
 *  返回所有损益类（费用类或收入类）科目
 * @param in    费用类（false）或收入类（true，默认）
 * @return
 */
QList<FirstSubject *> SubjectManager::getSyClsSubs(bool in)
{
    QList<FirstSubject *> fsubs;
    foreach(FirstSubject* fsub, fstSubHash.values()){
        if((fsub->getSubClass() == SC_SY) && ((fsub->getJdDir() && !in) || (!fsub->getJdDir() && in)))
            fsubs<<fsub;
    }
    return fsubs;
}

/**
 * @brief SubjectManager::getFstSubject
 * @param code  科目代码
 * @return
 */
FirstSubject *SubjectManager::getFstSubject(QString code)
{
    QHashIterator<int,FirstSubject*> it(fstSubHash);
    bool ok = false;
    while(it.hasNext()){
        it.next();
        if(it.value()->getCode() == code)
            return it.value();
    }
    if(!ok)
        return NULL;
}


SubjectManager::SubjectManager(Account *account, int subSys):
    account(account),subSys(subSys)
{
    dbUtil = account->getDbUtil();
    init();
}

/**
 * @brief 只在执行导入科目的操作后，调用此方法将导入的科目装载到对象中
 * @return
 */
bool SubjectManager::loadAfterImport()
{
    return dbUtil->initSubjects(this,subSys);
}

bool SubjectManager::save()
{
//    foreach(SubjectNameItem* ni, nameItems.values()){
//        if(ni->getEditState() != ES_NI_INIT && !dbUtil->saveNameItem(ni))
//            return false;
//    }
}

/**
 * @brief SubjectManager::getBankClsCode
 * @return
 */
int SubjectManager::getBankClsCode()
{

}


/**
 * @brief 获取未使用的名称条目类别代码
 * @return
 */
int SubjectManager::getNotUsedNiClsCode()
{
    int code = 0;
    foreach(int c, nameItemCls.keys()){
        if(code < c)
            code = c;
    }
    return ++code;
}

/**
 * @brief 添加新的名称条目类别
 * @param code      类别代码
 * @param name      类别名称
 * @param explain   类别解释信息
 * @return          如果新类别的代码与原有的存在冲突，则返回false
 */
bool SubjectManager::addNiClass(int code, QString name, QString explain)
{
    if(nameItemCls.contains(code))
       return false;
    nameItemCls[code] = QStringList();
    nameItemCls[code].append(name);
    nameItemCls[code].append(explain);
}

/**
 * @brief 修改指定名称条目类别
 * @param code
 * @param name
 * @param explain
 * @return
 */
bool SubjectManager::modifyNiClass(int code, QString name, QString explain)
{
    if(!nameItemCls.contains(code))
        return false;
    nameItemCls[code][0] = name;
    nameItemCls[code][1] = explain;
}

/**
 * @brief 指定代码的名称条目类别是否被使用
 * @param code
 * @return
 */
bool SubjectManager::isUsedNiCls(int code)
{
    foreach(SubjectNameItem* ni, nameItems){
        if(ni->getClassId() == code)
            return true;
    }
    return false;
}

/**
 * @brief 移除指定代码的名称条目类别
 * @param code
 */
bool SubjectManager::removeNiCls(int code)
{
    if(isUsedNiCls(code))
        return false;
    nameItemCls.remove(code);
    return true;
}


/**
 * @brief SubjectManager::removeNameItem
 *  移除名称条目
 * @param nItem
 */
void SubjectManager::removeNameItem(SubjectNameItem *nItem)
{
    if(!nItem)
        return;
    if(!nameItems.contains(nItem->getId()))
        return;
    delNameItems<<nItem;
    nameItems.remove(nItem->getId());
    nItem->setDelete(true);
}

/**
 * @brief SubjectManager::restoreNI
 * 恢复被删除的名称条目
 * @param nItem
 * @return
 */
bool SubjectManager::restoreNI(SubjectNameItem* nItem)
{
    for(int i = 0; i < delNameItems.count(); ++i){
        if(nItem == delNameItems.at(i)){
            nameItems[nItem->getId()] = delNameItems.takeAt(i);
            return true;
        }
    }
    return false;
}

/**
 * @brief SubjectManager::restoreNI
 * 恢复被删除的名称条目
 * @param sname
 * @param lname
 * @param remCode
 * @param nameCls
 * @return
 */
SubjectNameItem *SubjectManager::restoreNI(QString sname, QString lname, QString remCode, int nameCls)
{
    SubjectNameItem* ni;
    for(int i = 0; i < delNameItems.count(); ++i){
        ni = delNameItems.at(i);
        if(ni->getShortName() == sname &&
           ni->getLongName() == lname &&
           ni->getRemCode() == remCode &&
           ni->getClassId() == nameCls)
            ni->setDelete(false);
            nameItems[ni->getId()] = delNameItems.takeAt(i);
            return ni;
    }
    return NULL;
}


bool SubjectManager::init()
{
    FSub_NULL = new FirstSubject(this,-1,SC_NULL,QObject::tr("空"),"","",0,true);
    dbUtil->initSubjects(this,subSys);
}

/**
 * @brief SubjectManager::isSySubject
 *  是损益类科目吗
 * @param sid
 * @return
 */
bool SubjectManager::isSySubject(int sid)
{
    FirstSubject* sub = getFstSubject(sid);
    return sub->getSubClass() == SC_SY;
}

/**
 * @brief SubjectManager::isInSubject
 *  判断指定id的科目是否是损益类的费用类科目
 * @param sid   科目id
 * @param isFst 该科目是一级科目（默认）还是二级科目
 * @param yes   true：收入类，false：费用类
 * @return  true：表示指定的id是损益类科目
 */
bool SubjectManager::isSyClsSubject(int sid, bool &yes, bool isFst)
{
    FirstSubject* fsub;
    if(!isFst)
        fsub = getSndSubject(sid)->getParent();
    else
        fsub = getFstSubject(sid);
    if(!fsub){
        LOG_ERROR("fsub is null");
        return false;
    }
    if(fsub->getSubClass() != SC_SY)
        return false;
    yes = !fsub->getJdDir();
    return true;
}



/**
 * @brief 指定的二级科目是否在账户中已被采用了
 * @param ssub
 * @return
 */
bool SubjectManager::isUsedSSub(SecondSubject *ssub)
{
    return dbUtil->ssubIsUsed(ssub);
}

/**
 * @brief SubjectManager::addNameItem
 *  添加新的名称条目
 * @param sname     简称
 * @param lname     全称
 * @param rcode     助记符
 * @param clsId     名称类别代码
 * @param crtTime   创建时间
 * @param creator   创建者
 * @return
 */
SubjectNameItem *SubjectManager::addNameItem(QString sname, QString lname, QString rcode, int clsId, QDateTime crtTime, User *creator)
{
    SubjectNameItem* ni = new SubjectNameItem(0,clsId,sname,lname,rcode,crtTime,creator);
//    if(!dbUtil->saveNameItem(ni)){
//        delete ni;
//        return NULL;
//    }
    SubjectManager::nameItems[ni->getId()] = ni;
    return ni;
}

/**
 * @brief SubjectManager::addSndSubject
 *  使用指定名称条目对象在指定的一级科目下创建二级科目
 * @param fsub      主目对象
 * @param ni        所用的名称对象
 * @param code      二级科目的代码
 * @param weight    权重
 * @param isEnabled 是否启用
 * @param crtTime   创建时间
 * @param creator   创建者
 * @return
 */
SecondSubject *SubjectManager::addSndSubject(FirstSubject *fsub, SubjectNameItem *ni, QString code, int weight, bool isEnabled, QDateTime crtTime, User *creator)
{
    if(!ni){
        LOG_ERROR("SubjectNameItem object is NULL!");
        return NULL;
    }
    foreach(SecondSubject* sub, fsub->childSubs){
        if(sub->getNameItem() == ni){
            LOG_ERROR(QObject::tr("SecondSubject(%1) have existed in FirstSubject(%2)")
                      .arg(sub->getName()).arg(fsub->getName()));
            return NULL;
        }
    }

    SecondSubject* ssub = new SecondSubject(fsub,0,ni,code,weight,isEnabled,crtTime,QDateTime(),creator);
    if(!dbUtil->saveSndSubject(ssub)){
        delete ssub;
        return NULL;
    }
    fsub->childSubs<<ssub;
    sndSubs[ssub->getId()] = ssub;
    return ssub;
}

/**
 * @brief SubjectManager::getBankAccount
 *  获取与银行子目对象对应的银行对象
 * @param ssub
 * @return
 */
BankAccount *SubjectManager::getBankAccount(SecondSubject* ssub)
{
    foreach(BankAccount* ba, account->getAllBankAccount()){
        if(ba->niObj == ssub->getNameItem())
            return ba;
    }
    return NULL;
}

/**
 * @brief SubjectManager::getSubMatchMt
 *  获取与银行科目到子科目对应的货币对象
 * @param ssub
 * @return
 */
Money *SubjectManager::getSubMatchMt(SecondSubject *ssub)
{
    int mt = dbUtil->getBankSubMatchMoney(ssub);
    return account->getAllMoneys().value(mt);
}

/**
 * @brief SubjectManager::isBankSndSub
 *  判断二级科目是否是银行的子目
 * @param ssub
 * @return
 */
bool SubjectManager::isBankSndSub(SecondSubject *ssub)
{
    if(ssub->getParent() != bankSub)
        return false;
    else
        return true;
}













