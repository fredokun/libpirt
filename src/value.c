/**
 * @file value.c
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Dany SIRIPHOL
 * @author Joël HING
 * @author Loïc Girault
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <value_repr.h>
#include <channel_repr.h>
#include <atomic_repr.h>
#include <error.h>
#include <tools.h>



/******************************
 * Immediate values : No value *
 ******************************/

static PICC_NoValue picc_novalue = { MAKE_HEADER(TAG_NOVALUE, 0) };

PICC_Value *PICC_create_no_value()
{
    PICC_Value * val = (PICC_Value*) &picc_novalue;

    #ifdef CONTRACT_POST_INV
        PICC_NoValue_inv((PICC_NoValue*)val);
    #endif

    return val;
}

void PICC_NoValue_inv(PICC_NoValue * val)
{
    ASSERT(val != NULL);
    int tag = GET_VALUE_TAG(val->header);
    int control = GET_VALUE_CTRL(val->header);
    ASSERT(tag == TAG_NOVALUE);
    ASSERT(control == 0);
}

/******************************
 * Immediate values : boolean *
 ******************************/

static PICC_BoolValue picc_true = {MAKE_HEADER(TAG_BOOLEAN, true)};
static PICC_BoolValue picc_false = {MAKE_HEADER(TAG_BOOLEAN, false)};

PICC_Value *PICC_create_bool_value(bool boolean)
{
    PICC_BoolValue * val;
    if (boolean) {
    	val = &picc_true;
    } else {
    	val = &picc_false;
    }

    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(val);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(GET_VALUE_CTRL(val->header) == boolean);
    #endif

    return (PICC_Value*) val;
}


bool PICC_bool_of_bool_value(PICC_Value *val)
{
    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv((PICC_BoolValue*) val);
    #endif


    #ifdef CONTRACT_PRE
        ASSERT(IS_BOOLEAN(val));

    #endif

    return (PICC_BoolValue*) val == &picc_true;
}


void PICC_BoolValue_inv(PICC_BoolValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    int control = GET_VALUE_CTRL(val->header);
    ASSERT(tag == 2 );
    ASSERT( control == true || control == false);
}

// boolean primitives

void PICC_Bool_and( PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_BoolValue * bv1= (PICC_BoolValue*) v1;
    PICC_BoolValue * bv2= (PICC_BoolValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif


    int bres = GET_VALUE_CTRL(bv1->header) & GET_VALUE_CTRL(bv2->header);

    PICC_INIT_BOOL_VALUE(res, bres);

    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
        PICC_BoolValue_inv((PICC_BoolValue*) res);
    #endif

    #ifdef CONTRACT_POST
        //post
        int result = GET_VALUE_CTRL(((PICC_BoolValue*) res)->header);
        ASSERT( result == bres );
    #endif
}

void PICC_Bool_or ( PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_BoolValue * bv1= (PICC_BoolValue*) v1;
    PICC_BoolValue * bv2= (PICC_BoolValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif


    int bres = GET_VALUE_CTRL(bv1->header) | GET_VALUE_CTRL(bv2->header);

    PICC_INIT_BOOL_VALUE(res, bres);


    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
        PICC_BoolValue_inv((PICC_BoolValue*) res);
    #endif

    #ifdef CONTRACT_POST
        //post
       int result = GET_VALUE_CTRL(((PICC_BoolValue*) res)->header);
       ASSERT( result == bres );
    #endif
}


void PICC_Bool_xor( PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_BoolValue * bv1= (PICC_BoolValue*) v1;
    PICC_BoolValue * bv2= (PICC_BoolValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif


    int bres = GET_VALUE_CTRL(bv1->header) ^ GET_VALUE_CTRL(bv2->header);

    PICC_INIT_BOOL_VALUE(res, bres);

    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
        PICC_BoolValue_inv((PICC_BoolValue*) res);
    #endif

    #ifdef CONTRACT_POST
        //post
       int result = GET_VALUE_CTRL(((PICC_BoolValue*) res)->header);
       ASSERT( result == bres );
    #endif
}


void PICC_Bool_not( PICC_Value *res, PICC_Value *v)
{
    PICC_BoolValue * bv= (PICC_BoolValue*) v;

    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv);
    #endif


    int bres = ! GET_VALUE_CTRL(bv->header);

    PICC_INIT_BOOL_VALUE(res, bres);

    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv);
        PICC_BoolValue_inv((PICC_BoolValue *)res);
    #endif

   #ifdef CONTRACT_POST
        //post
       int result = GET_VALUE_CTRL(((PICC_BoolValue*) res)->header);
       ASSERT( result == bres );
    #endif
}

/******************************
 * Immediate values : integer *
 ******************************/

PICC_Value * PICC_create_int_value(int data)
{
    PICC_ALLOC_CRASH(val, PICC_IntValue) {
        val->header = MAKE_HEADER(TAG_INTEGER, 0);
        val->data = data;
    }

    #ifdef CONTRACT_POST_INV
            PICC_IntValue_inv(val);
    #endif

    return (PICC_Value *) val;
}

PICC_IntValue * PICC_free_int(PICC_IntValue * val)
{
    free(val);
    val = NULL;
    return val;
}

void PICC_IntValue_inv(PICC_IntValue * val)
{
    ASSERT(val != NULL);
    int tag = GET_VALUE_TAG(val->header);
    ASSERT(tag == TAG_INTEGER )
}

void PICC_Int_add (PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{

    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //capture
        int value1_at_pre = iv1->data;
        int value2_at_pre = iv2->data;
    #endif

    int result = iv1->data + iv2->data;

    PICC_INIT_INT_VALUE(res, result);

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv((PICC_IntValue*) res);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(((PICC_IntValue*)res)->data == value1_at_pre + value2_at_pre);
    #endif
}

void PICC_Int_multiply (PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //capture
        int value1_at_pre = iv1->data;
        int value2_at_pre = iv2->data;
    #endif

    int r = iv1->data * iv2->data;

    PICC_INIT_INT_VALUE(res, r);

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
        PICC_IntValue_inv((PICC_IntValue*) res);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(((PICC_IntValue*)res)->data == value1_at_pre * value2_at_pre);
    #endif
}

void PICC_Int_divide (PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //capture
        int value1_at_pre = iv1->data;
        int value2_at_pre = iv2->data;
    #endif

    int r = iv1->data / iv2->data;

    PICC_INIT_INT_VALUE(res, r);

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(((PICC_IntValue*)res)->data == value1_at_pre / value2_at_pre);
    #endif
}

void PICC_Int_substract(PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //capture
        int value1_at_pre = iv1->data;
        int value2_at_pre = iv2->data;
    #endif

    int r = iv1->data - iv2->data;

    PICC_INIT_INT_VALUE(res, r);

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(((PICC_IntValue*)res)->data == value1_at_pre - value2_at_pre);
    #endif
}

void PICC_Int_less_than(PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //capture
        int value1_at_pre = iv1->data;
        int value2_at_pre = iv2->data;
    #endif

    int r = iv1->data < iv2->data;

    PICC_INIT_BOOL_VALUE(res, r);

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(PICC_BOOL_OF_BOOL_VALUE(res) == (value1_at_pre < value2_at_pre) );
    #endif
}

void PICC_Int_modulo(PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

    #ifdef CONTRACT_PRE_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //capture
        int value1_at_pre = iv1->data;
        int value2_at_pre = iv2->data;
    #endif

    int r = iv1->data % iv2->data;

    PICC_INIT_INT_VALUE(res, r);

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
  	ASSERT(((PICC_IntValue*)res)->data == value1_at_pre % value2_at_pre);
  #endif
}

/******************************
 * Immediate values : float *
 ******************************/

//TODO

/******************
 * Tuples values  *
******************/

PICC_Value *PICC_create_tuple_value(int size) {

    #ifdef CONTRACT_PRE
       //pre
 		ASSERT(size >= 0);
    #endif

    PICC_ALLOC_CRASH(val, PICC_TupleValue) {
        val->header = MAKE_HEADER(TAG_TUPLE, size);
        val->elements = malloc(sizeof(PICC_Value *)*size);
        val->size = size;
    }


    #ifdef CONTRACT_POST_INV
        PICC_TupleValue_inv(val);
    #endif

    #ifdef CONTRACT_POST
        ASSERT(val->size == size)
    #endif

    return (PICC_Value *)val;
}

PICC_TupleValue * PICC_free_tuple_value(PICC_TupleValue * tup) {
   free(tup->elements);
   free(tup);
   return NULL;
}

void PICC_TupleValue_inv(PICC_TupleValue *tuple)
{
    ASSERT(tuple != NULL );
    int tag = GET_VALUE_TAG(tuple->header);
    ASSERT(tag == TAG_TUPLE );
    ASSERT(tuple->size >=0);
    if(tuple->size >0)
        ASSERT(tuple->elements !=NULL);
}

void PICC_set_tuple_elements(PICC_Value *val, PICC_Value **values)
{
    PICC_TupleValue * tuple = (PICC_TupleValue*) val;

    #ifdef CONTRACT_PRE_INV
        PICC_TupleValue_inv(tuple);

    #endif

    #ifdef CONTRACT_PRE
        ASSERT(values!= NULL);
    #endif

    tuple->elements = values;

    #ifdef CONTRACT_POST_INV
        PICC_TupleValue_inv(tuple);
    #endif
}

PICC_Value *PICC_get_tuple_element(PICC_Value *val, int index)
{
    PICC_TupleValue * tuple = (PICC_TupleValue*) val;

    #ifdef CONTRACT_PRE_INV
        PICC_TupleValue_inv(tuple);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(index>=0);
        ASSERT(index<tuple->size);
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_TupleValue_inv(tuple);
    #endif

    return tuple->elements[index];
}

bool PICC_copy_tuple(PICC_Value **to, PICC_TupleValue* from){

    #ifdef CONTRACT_PRE_INV
        PICC_TupleValue_inv(from);
    #endif

    PICC_TupleValue **tuple = (PICC_TupleValue**) to;

    *to = PICC_create_tuple_value(from->size);

    int i;
    for(i=0;i<from->size;i++)
    {
        (*tuple)->elements[i] = from->elements[i];
    }

    #ifdef CONTRACT_PRE_INV
        PICC_TupleValue_inv(from);
        PICC_TupleValue_inv(*tuple);
    #endif

    #ifdef CONTRACT_POST
        ASSERT((*tuple)->size == from->size );
        for(i=0;i<from->size;i++)
        {
            ASSERT((*tuple)->elements[i] == from->elements[i]);
        }
    #endif

    return true;
}

/******************
 * String values  *
 ******************/


void PICC_string_handle_reclaimer(PICC_StringHandle *handle, PICC_Error* e){
    free(handle->data);
    //PICC_lock_free(handle->lock); see comment in gc.c - handle_dec_ref_count
    free(handle);
}

PICC_StringHandle *PICC_create_string_handle(char *string)
{
    #ifdef CONTRACT_PRE
        // pre
        ASSERT(string != NULL);
    #endif

    PICC_ALLOC_CRASH(val, PICC_StringHandle) {
        val->global_rc = 1;
	val->lock = PICC_create_lock(NULL);
	val->reclaim= (PICC_Reclaimer)PICC_string_handle_reclaimer;
        val->data = malloc(sizeof(char)*strlen(string) +1);
        strcpy(val->data, string);
    }

    #ifdef CONTRACT_POST_INV
        PICC_StringHandle_inv(val);
    #endif

    return val;
}


void PICC_StringHandle_inv(PICC_StringHandle *handle)
{
    ASSERT(handle != NULL);
    ASSERT(handle->global_rc >= 0);
}


PICC_StringValue *PICC_create_empty_string_value()
{
    PICC_StringValue *val = malloc(sizeof(PICC_StringValue));
    val->header = MAKE_HEADER(TAG_STRING, 0);
    val->data = NULL;

    #ifdef CONTRACT_POST_INV
        PICC_StringValue_inv(val);
    #endif

    return val;
}

PICC_Value *PICC_create_string_value( char *string )
{
    #ifdef CONTRACT_PRE
        // pre
        ASSERT(string != NULL);
    #endif

    PICC_StringValue *val = PICC_create_empty_string_value();
    ASSERT(val != NULL);
    PICC_StringHandle *handle = PICC_create_string_handle(string);
    ASSERT(handle != NULL);

    val->data = handle;

    #ifdef CONTRACT_POST_INV
        PICC_StringHandle_inv(handle);
        PICC_StringValue_inv(val);
    #endif

    return (PICC_Value*)val;
}

PICC_StringValue *PICC_free_string( PICC_StringValue *string )
{
    //the handle will is managed with dec_ref / incr_ref functions
    /* if(string->handle != NULL) */
    /*     PICC_free_string_handle(string->handle); */
    free(string);
    return (string = NULL);
}

bool PICC_copy_string(PICC_Value **to, PICC_StringValue* from){


    #ifdef CONTRACT_PRE_INV
        PICC_StringValue_inv(from);
    #endif

    PICC_StringValue** strto = (PICC_StringValue**) to;

    *strto = PICC_create_empty_string_value();
    (*strto)->data = from->data;


    #ifdef CONTRACT_POST_INV
        PICC_StringValue_inv(from);
        PICC_StringValue_inv(*strto);
    #endif

    #ifdef CONTRACT_POST
        ASSERT(strcmp(from->data->data, (*strto)->data->data) == 0 );
    #endif

    return true;
}

void PICC_StringValue_inv(PICC_StringValue *string)
{
    ASSERT(string != NULL);
    int tag = GET_VALUE_TAG(string->header);
    ASSERT(tag == TAG_STRING );
    if(string->data != NULL)
        PICC_StringHandle_inv(string->data);
}

/******************
 * Channel values  *
 ******************/



PICC_ChannelValue *PICC_create_empty_channel_value( PICC_ChannelKind kind )
{

    PICC_ChannelValue *val = malloc(sizeof( PICC_ChannelValue));
    ASSERT(val != NULL);
    val->header = MAKE_HEADER(TAG_CHANNEL, kind);

    #ifdef CONTRACT_POST_INV
        int tag = GET_VALUE_TAG(val->header);
        ASSERT(tag == TAG_CHANNEL);
    #endif

    return val;
}



/* PICC_Value *PICC_create_pi_channel_value() */
/* { */
/*     return (PICC_Value*) PICC_create_channel_value(PI_CHANNEL); */
/* } */

PICC_Value *PICC_create_channel_value(PICC_Channel* channel)
{

    #ifdef CONTRACT_PRE_INV
        PICC_Channel_inv(channel);
    #endif

    PICC_ChannelValue *val = PICC_create_empty_channel_value(PI_CHANNEL);
    val->data = (PICC_ChannelHandle*)channel;

    #ifdef CONTRACT_POST_INV
        PICC_ChannelValue_inv(val);
    #endif

    return (PICC_Value*) val;
}

PICC_Channel *PICC_channel_of_channel_value(PICC_Value* channel)
{
    #ifdef CONTRACT_PRE
        ASSERT(IS_CHANNEL(channel));
    #endif

    #ifdef CONTRACT_PRE_INV
        PICC_ChannelValue_inv((PICC_ChannelValue*) channel);
    #endif

    return (PICC_Channel*) ((PICC_ChannelValue*) channel)->data;
}

/* not finished. should use reclaim channel when possible */

PICC_ChannelValue *PICC_free_channel_value( PICC_ChannelValue *channel)
{
    free(channel);
    return NULL;
}

bool PICC_copy_channel(PICC_Value **to, PICC_ChannelValue *from){

    #ifdef CONTRACT_PRE_INV
        PICC_ChannelValue_inv(from);
    #endif

    PICC_ChannelValue **channel = (PICC_ChannelValue**) to;

    	*channel = PICC_create_empty_channel_value( PI_CHANNEL );
    	(*channel)->data = from->data;


    #ifdef CONTRACT_POST_INV
        PICC_ChannelValue_inv(*channel);
        PICC_ChannelValue_inv(from);
    #endif

    #ifdef CONTRACT_POST
        ASSERT((*channel)->data == from->data );
    #endif

    return true;
}


void PICC_ChannelValue_inv(PICC_ChannelValue *channel)
{
    ASSERT(channel != NULL);
    int tag = GET_VALUE_TAG(channel->header);
    int ctrl = GET_VALUE_CTRL(channel->header);
    ASSERT(tag == TAG_CHANNEL );
    if(channel->data != NULL)
        if(ctrl == PI_CHANNEL)
            PICC_Channel_inv(channel->data);

}

/**********************************
 * user defined immediate values  *
 **********************************/

/* PICC_ImmediateValue *PICC_create_immediate_value(int size) */
/* { */
/*     #ifdef CONTRACT_PRE */
/*         //pre */
/* 		ASSERT(size >= 0); */
/*     #endif */

/*     PICC_ImmediateValue *val = malloc(sizeof( PICC_ImmediateValue)); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate immediate"); */
/*     } */
/*     val->header = MAKE_HEADER(TAG_USER_DEFINED_IMMEDIATE, 0); */
/*     val->data = malloc(sizeof( int) * size); */
/*     if (val== NULL) { */
/*         abort_with_message("Cannot allocate immediate"); */
/*     } */

/*     #ifdef CONTRACT_POST_INV */
/*         PICC_ImmediateValue_inv(val); */
/*     #endif */

/*     return val; */
/* } */

/* PICC_ImmediateValue *PICC_free_immediate_value( PICC_ImmediateValue *value) */
/* { */
/*     free(value->data); */
/*     free(value); */
/*     return NULL; */
/* } */

/* void PICC_ImmediateValue_inv(PICC_ImmediateValue *value) */
/* { */
/*     ASSERT(value != NULL); */
/*     int tag = GET_VALUE_TAG(value->header); */
/*     ASSERT(tag ==  TAG_USER_DEFINED_IMMEDIATE ); */
/* } */


/**********************************
 * user defined managed values  *
 **********************************/

/* PICC_ManagedValue *PICC_create_managed_value(int size) */
/* { */

/*     #ifdef CONTRACT_PRE */
/*         //pre */
/* 		ASSERT(size >= 0); */
/*     #endif */

/*     PICC_ManagedValue *val = malloc(sizeof( PICC_ManagedValue)); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate managed"); */
/*     } */
/*     val->header = MAKE_HEADER(TAG_USER_DEFINED_MANAGED, 0); */
/*     val->data = malloc(sizeof( int) * size); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate managed"); */
/*     } */

/*     #ifdef CONTRACT_POST_INV */
/*         PICC_ManagedValue_inv(val); */
/*     #endif */

/*     return val; */
/* } */

/* PICC_ManagedValue *PICC_free_managed_value( PICC_ManagedValue *value) */
/* { */
/*     free(value->data); */
/*     free(value); */
/*     return NULL; */
/* } */

/* void PICC_ManagedValue_inv(PICC_ManagedValue *value) */
/* { */
/*     ASSERT(value != NULL); */
/*     int tag = GET_VALUE_TAG(value->header); */
/*     ASSERT(tag ==  TAG_USER_DEFINED_MANAGED ); */
/* } */

/* void PICC_copy_value(PICC_Value *to, PICC_Value *from){ */

/*    if(to!=NULL) */
/*       PICC_free(to); */


/* } */


/*******************************
 *  compares 2 values          *
 *******************************/
void PICC_equals(PICC_Value *res, PICC_Value * value1, PICC_Value * value2){
    int c = PICC_compare_values(value1, value2);
    if (c == 0) c = 1;
    else c = 0;

    PICC_INIT_BOOL_VALUE(res, c);
}

int PICC_compare_values(PICC_Value * value1, PICC_Value * value2)
{
    PICC_TagValue tag1 = GET_VALUE_TAG(value1->header);
    int ctrl1 = GET_VALUE_CTRL(value1->header);

    PICC_TagValue tag2 = GET_VALUE_TAG(value2->header);
    int ctrl2 = GET_VALUE_CTRL(value2->header);

    if(tag1 != tag2){
        return -1;
    }

    switch(tag1) {
        case TAG_RESERVED: {
            break;
        }
        case TAG_NOVALUE: {
            break;
        }
        case TAG_INTEGER: {
            PICC_IntValue *int_value1 = (PICC_IntValue *) value1;
            PICC_IntValue *int_value2 = (PICC_IntValue *) value2;
            if(int_value1->data < int_value2->data){
                // value1 less than value2
                return -1;
            }
            if(int_value1->data > int_value2->data){
                // value1 greater than value2
                return 1;
            }
            // value1 equals value2
            return 0;
            break;
        }
        case TAG_FLOAT: {
            PICC_FloatValue *float_value1 = (PICC_FloatValue *) value1;
            PICC_FloatValue *float_value2 = (PICC_FloatValue *) value2;
            if(float_value1->data < float_value2->data){
                // value1 less than value2
                return -1;
            }
            if(float_value1->data > float_value2->data){
                // value1 greater than value2
                return 1;
            }
            // value1 equals value2
            return 0;
            break;
        }
        case TAG_BOOLEAN: {
            if (ctrl1 == ctrl2) {
                // both true or both false
                return 0;
            }
            // different bool
            return -1;
            break;
        }/*
        case TAG_TUPLE: {
            PICC_TupleValue *tup1 = (PICC_TupleValue *) value1;
            PICC_TupleValue *tup2 = (PICC_TupleValue *) value2;
            if(ctrl1 != ctrl2){
                return -1;
            }
            if(ctrl1 == 0){
                return 0;
            }

            for(int i=0;i<ctrl1;i++) {
                int res = compare_values(tup1->elements[i], tup2->elements[i]) ;
                if(res != 0){
                    return res;
                }
            }
            // same tuples
            return 0;
            break;
        }
        */
        case TAG_STRING: {
            return strcmp(((PICC_StringValue *)value1)->data->data, ((PICC_StringValue *)value2)->data->data);
            break;
        }
        case TAG_CHANNEL: {
            PICC_Channel *ch1 = PICC_channel_of_channel_value(value1);
            PICC_Channel *ch2 = PICC_channel_of_channel_value(value2);
            if(ch1 == ch2)
				return 0;
            if(ch1 > ch2)
                return -1;
            else
                return 1;
            break;
        }

        default:
            break;
    }

    return -1;
}

/***** utilities ********/

PICC_Value* PICC_free_value(PICC_Value *v)
{
    if (v==NULL) return NULL;

    switch(GET_VALUE_TAG(v->header)) {
        case TAG_RESERVED:
        case TAG_NOVALUE:
        case TAG_BOOLEAN:
    	    return NULL;
        case TAG_INTEGER:
    	    return (PICC_Value*) PICC_free_int((PICC_IntValue*) v);
        case TAG_STRING:
    	    return (PICC_Value*) PICC_free_string((PICC_StringValue*) v);
        case TAG_CHANNEL:
        	return (PICC_Value*) PICC_free_channel_value((PICC_ChannelValue*) v);
        case TAG_TUPLE:
            return (PICC_Value*) PICC_free_tuple_value((PICC_TupleValue*) v);
        /*TODO*/
        case TAG_FLOAT:
        case TAG_USER_DEFINED_IMMEDIATE:
        case TAG_USER_DEFINED_MANAGED:
    	return NULL;
        default:
    	return NULL;
    }
}

bool PICC_copy_value(PICC_Value **to, PICC_Value *from) {

    #ifdef CONTRACT_PRE
        ASSERT(to != NULL ); // && *to != NULL); *to can be NULL !! the new value is allocated
        ASSERT(from != NULL);
    #endif
    PICC_free_value(*to);
    switch(GET_VALUE_TAG(from->header)) {
        case TAG_RESERVED:
            *to = NULL;
            return true;
        case TAG_NOVALUE:
            *to = (PICC_Value*) &picc_novalue;
            return true;
        case TAG_BOOLEAN:
            if(GET_VALUE_CTRL(from->header))
                *to = (PICC_Value*) &picc_true;
            else
                *to = (PICC_Value*) &picc_false;
    	    return true;
        case TAG_INTEGER:
            *to = PICC_create_int_value( ((PICC_IntValue*) from)->data );
    	    return true;
        case TAG_STRING:
            PICC_copy_string(to,(PICC_StringValue *)from);
            return true;
        case TAG_CHANNEL:
        	PICC_copy_channel(to,(PICC_ChannelValue *)from);
            return true;
        case TAG_TUPLE:
            PICC_copy_tuple(to,(PICC_TupleValue *)from);
            return true;
    	/*TODO*/
        case TAG_FLOAT:

        case TAG_USER_DEFINED_IMMEDIATE:
        case TAG_USER_DEFINED_MANAGED:
    	return true;
        default:
    	return false;

    return true;
    }
}


/**** Example of dispatch function *****/

void PICC_print_value_infos(PICC_Value * value)
{
    PICC_TagValue tag = GET_VALUE_TAG(value->header);
    int ctrl = GET_VALUE_CTRL(value->header);

    printf("------------\n");
    printf("tag = %d\n", GET_VALUE_TAG(value->header));
    printf("ctrl = %d\n", GET_VALUE_CTRL(value->header));

    switch(tag) {
        case TAG_INTEGER: {
            printf("Type: integer\n");
            PICC_IntValue *int_value = (PICC_IntValue *) value;
            printf("Value = %d\n", int_value->data);
            break;
        }
        case TAG_BOOLEAN: {
            printf("Type: boolean\n");
            if (ctrl == 0) {
                printf("Value = False\n");
            } else if(ctrl == 1) {
                printf("Value = True\n");
            } else {
                printf("Unknown boolean value\n");
                abort();
            }
            break;
        }

        case TAG_TUPLE: {
            printf("Type: tuple\n");
            PICC_TupleValue *tup = (PICC_TupleValue *) value;
            for(int i=0;i<ctrl;i++) {
                printf("%d-th element>>>>>>>>>\n",i);
                PICC_print_value_infos(tup->elements[i]);
                printf("<<<<<<<<<<<\n");
			}
		}
        case TAG_STRING:
            printf("%s\n", ((PICC_StringValue *)value)->data->data );
            break;
	case TAG_CHANNEL:
	    printf("Channel gloabl_rc = %d\n", ((PICC_Channel *) value->data)->global_rc);
	    break;
        case TAG_FLOAT:
        case TAG_USER_DEFINED_IMMEDIATE:
        case TAG_USER_DEFINED_MANAGED:
            printf("not implemented");
            break;
        default:
            printf("unknown tag\n");
            abort();
            break;
    }
}

void PICC_print_value(PICC_Value * value)
{
    PICC_TagValue tag = GET_VALUE_TAG(value->header);
    int ctrl = GET_VALUE_CTRL(value->header);

    switch(tag) {
        case TAG_INTEGER: {
            PICC_IntValue *int_value = (PICC_IntValue *) value;
            printf("%d", int_value->data);
            break;
        }
        case TAG_BOOLEAN: {
            if (ctrl == 0) {
                printf("false");
            } else if(ctrl == 1) {
                printf("true");
            } else {
                printf("Invalid boolean");
                abort();
            }
            break;
        }
        case TAG_STRING:
            printf("%s", ((PICC_StringValue *)value)->data->data );
            break;
        case TAG_FLOAT:
        case TAG_USER_DEFINED_IMMEDIATE:
        case TAG_USER_DEFINED_MANAGED:
            printf("not implemented");
            break;
    case TAG_RESERVED:
            printf("reserved tag used (please report)\n");
            abort();
            break;
        default:
            printf("unknown tag (please report)\n");
            abort();
            break;
    }
}
