/*
 * ad_ai.h
 *
 *  Created on: 2021Äê3ÔÂ16ÈÕ
 *      Author: HP
 */

#ifndef SOURCE_AD_AI_H_
#define SOURCE_AD_AI_H_
#include "head_file.h"
extern const unsigned char model1[];
typedef struct _model_info_struct
{
    char author[30];
    int quant_bits;
    int frac_bits;
    const char* name_string;
}model_info_struct;
model_info_struct inf;

extern void get_model_info(const void *in_buf, model_info_struct *inf);
extern void* run_model(const void *model_buf, const void *in_buf, signed short int *out1_buf);


#endif /* SOURCE_AD_AI_H_ */
