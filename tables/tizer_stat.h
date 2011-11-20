#include"fields.h"
struct __attribute__((__packed__)) table1 {
    int32_t date;
    int32_t tizer_id;
    int32_t partner_id;
    int32_t site_id;
    int32_t site_area_id;
    int32_t area_format_id;
    int32_t targeting_id;
    int32_t media_type;
    double  cpc;
    double  gain;
    double  sale_cost;
    int64_t block_show;
    int64_t real_show;
    int64_t show;
    int32_t click;
    int32_t multiplicator_click;
    int32_t spam_click;
    int32_t campaign_id;
    int32_t depth;
    int32_t arrived;
    int32_t adv_site_id;
    int32_t content_cat_id;
    int32_t rekl_id;
    int32_t partner_site_id;
    int32_t deleted;

    void dump() {
//        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%.2f\tbs=%ld\t"
//               "%ld\ts=%ld\tc=%d\tmc=%d\tspam=%d\t%d\td=%d\ta=%d\t%d\t%d\t%d\t%d\t%d\n",

        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%.2f\t%ld\t"
               "%ld\t%ld\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
             date,               
             tizer_id,           
             partner_id,         
             site_id,            
             site_area_id,       
             area_format_id,     
             targeting_id,       
             media_type,         
             cpc,                
             gain,               
             sale_cost,          
             block_show,         
             real_show,          
             show,               
             click,              
             multiplicator_click,
             spam_click,         
             campaign_id,        
             depth,              
             arrived,            
             adv_site_id,        
             content_cat_id,     
             rekl_id,            
             partner_site_id,    
             deleted
        );
    }


    int cmp(const table1 &right) const
    {
        if (date<right.date)                    return -1;
        if (date>right.date)                    return +1;
        if (tizer_id<right.tizer_id)            return -1;
        if (tizer_id>right.tizer_id)            return +1;
        if (partner_id<right.partner_id)        return -1;
        if (partner_id>right.partner_id)        return +1;
        if (site_id<right.site_id)              return -1;
        if (site_id>right.site_id)              return +1;
        if (site_area_id<right.site_area_id)    return -1;
        if (site_area_id>right.site_area_id)    return +1;
        if (area_format_id<right.area_format_id)return -1;
        if (area_format_id>right.area_format_id)return +1;
        if (targeting_id<right.targeting_id)    return -1;
        if (targeting_id>right.targeting_id)    return +1;
        if (media_type<right.media_type)        return -1;
        if (media_type>right.media_type)        return +1;

        return 0;
    };

    void merge(const table1 &right){
        cpc                 += right.cpc;
        gain                += right.gain;
        sale_cost           += right.sale_cost;
        block_show          += right.block_show;
        real_show           += right.real_show;
        show                += right.show;
        click               += right.click;
        multiplicator_click += right.multiplicator_click;
        spam_click          += right.spam_click;

        if (right.campaign_id > 0){
            campaign_id =  right.campaign_id;
        }

        depth               += right.depth;
        arrived             += right.arrived;

        if (right.adv_site_id > 0){
            adv_site_id = right.adv_site_id;
        }

        if (right.content_cat_id > 0){
            content_cat_id = right.content_cat_id;
        }

        if (right.rekl_id > 0){
            rekl_id = right.rekl_id;
        }

        if (right.partner_site_id > 0){
            partner_site_id = right.partner_site_id;
        }
    }

};


struct tableFields  table1Fields[] ={
    {"date", offsetof(table1,date)                               ,tableFields::INT32  },
    {"tizer_id",offsetof(table1,tizer_id)                        ,tableFields::INT32  },
    {"partner_id",offsetof(table1,partner_id)                    ,tableFields::INT32  },
    {"site_id",offsetof(table1,site_id)                          ,tableFields::INT32  },
    {"site_area_id",offsetof(table1,site_area_id)                ,tableFields::INT32  },
    {"area_format_id",offsetof(table1,area_format_id)            ,tableFields::INT32  },
    {"targeting_id",offsetof(table1,targeting_id)                ,tableFields::INT32  },
    {"media_type",offsetof(table1,media_type)                    ,tableFields::INT32  },
    {"cpc",offsetof(table1,cpc)                                  ,tableFields::DOUBLE },
    {"gain",offsetof(table1,gain)                                ,tableFields::DOUBLE },
    {"sale_cost",offsetof(table1,sale_cost)                      ,tableFields::DOUBLE },
    {"block_show",offsetof(table1,block_show)                    ,tableFields::INT64  },
    {"real_show",offsetof(table1,real_show)                      ,tableFields::INT64  },
    {"show",offsetof(table1,show)                                ,tableFields::INT64  },
    {"click",offsetof(table1,click)                              ,tableFields::INT32  },
    {"multiplicator_click",offsetof(table1,multiplicator_click)  ,tableFields::INT32  },
    {"spam_click",offsetof(table1,spam_click)                    ,tableFields::INT32  },
    {"campaign_id",offsetof(table1,campaign_id)                  ,tableFields::INT32  },
    {"depth",offsetof(table1,depth)                              ,tableFields::INT32  },
    {"arrived",offsetof(table1,arrived)                          ,tableFields::INT32  },
    {"adv_site_id",offsetof(table1,adv_site_id)                  ,tableFields::INT32  },
    {"content_cat_id",offsetof(table1,content_cat_id)            ,tableFields::INT32  },
    {"rekl_id",offsetof(table1,rekl_id)                          ,tableFields::INT32  },
    {"partner_site_id",offsetof(table1,partner_site_id)          ,tableFields::INT32  },
    {"deleted",offsetof(table1,deleted)                          ,tableFields::INT32  }
};
