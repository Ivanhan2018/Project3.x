#ifndef __TexasPoker__GoodsModel__
#define __TexasPoker__GoodsModel__

#include "Inc.h"

class GoodsModel
{
    
public:
    GoodsModel();
    ~GoodsModel();

public:
    /*
     id: 商品id,
     price:价格
     p_type:价格的类型 1:钻石,2:筹码,3:兑奖券
     property:兑换筹码的数量
     give_chip:赠送筹码数量
     type:物品的类别 2:兑换 3 限购 4 充值
     sort:排序
     icon: "shop-item06",
     new: 1,
     limit_num -1 不限数量
     limit_nextid -1 没有下一个物品
     
     id: 1,
     name: "25000筹码",
     price: 5,
     p_type: 1,
     property: 25000,
     give_chip: 0,
     type: 2,
     sort: 1,
     icon: "shop-item01",
     new: 0,
     limit_num: -1,
     limit_nextid: -1
     */
    UNS_INT       id;
    UNS_INT       price;
    UNS_INT       p_type;
    UNS_INT       property;
    UNS_INT       give_chip;
    UNS_INT       type;
    UNS_INT       sort;
    std::string   icon;
    UNS_INT       isNew;
    int           limit_num;
    int           limit_nextid;
    
};

#endif
