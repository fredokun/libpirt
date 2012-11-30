#ifndef PI_THREAD_H
#define PI_THREAD_H

#include <stdbool.h>

typedef struct PIT_SchedPool PIT_SchedPool;
typedef struct PIT_PiThread PIT_PiThread;
typedef struct PIT_Channel PIT_Channel;
typedef struct PIT_Commit PIT_Commit;
typedef struct PIT_InCommit PIT_InCommit;
typedef struct PIT_OutCommit PIT_OutCommit;
typedef struct PIT_Clock PIT_Clock;
typedef struct PIT_Value PIT_Value;
typedef struct PIT_Commit PIT_Commit;

typedef struct PIT_ConcurentReadyQueue *PIT_ConcurentReadyQueue;
typedef struct PIT_ConcurentWaitQueue *PIT_ConcurentReadyQueue;
typedef struct PIT_Mutex *PIT_Mutex;
typedef struct PIT_Condition *PIT_Condition;
typedef struct PIT_KnownsSet *PIT_KnownsSet;


typedef char *PIT_Label;
typedef void (*PIT_Function)(void);
typedef void (*PIT_EvalFunction)(PIT_PiThread);
typedef bool PIT_AtomicBoolean;
typedef int PIT_AtomicInt;

typedef enum
{
	IN_COMMIT,
	OUT_COMMIT
} PIT_CommitType;

typedef enum {
  STATUS_RUN,
  STATUS_CALL,
  STATUS_WAIT,
  STATUS_ENDED
} PIT_StatusKind;

typedef enum {
  INT_VAL,
  FLOAT_VAL,
  STRING_VAL,
  BOOL_VAL,
  CHANNEL_VAL,
} PIT_ValueKind;

struct
{
	PIT_CommitType type;
	PIT_PiThread *thread;
	PIT_Clock clock;
	int clockval;
	int cont_pc;
	PIT_Channel *channel;

	union
	{
		PIT_incommit in;
		PIT_outcommit out;
	}
} PIT_Commit;

struct PIT_SchedPool {
  PIT_ConcurentReadyQueue ready;
  PIT_ConcurentWaitQueue wait;
  PIT_MUTEX lock;
  PIT_Condition cond;
  int nb_slaves;
  int nb_waiting_slaves;
};

struct PIT_PiThread {
  PIT_StatusKind status;
  bool enable[];
  int enable_length;
  PIT_KnownsSet knowns;
  PIT_Value env[];
  int env_length;
  PIT_Commit commit;
  PIT_Commit* commits;
  PIT_Function proc;
  PIT_Label pc;
  int fuel;
  PIT_Mutex lock;
};

struct PIT_Channel {
  PIT_Commit* incommits;
  PIT_Commit* outcommits;
  int global_rc;
  PIT_AtomicBoolean lock;
};

struct PIT_InCommit {
  int refvar;
};

struct PIT_OutCommit {
  PIT_EvalFunction eval_func;
};

struct PIT_Clock {
  PIT_AtomicInt val;
};

struct PIT_Value {
  PIT_ValueKind kind;
  union {
    int as_int;
    double as_float;
    char *as_string;
    bool as_bool;
    PIT_Channel *as_channel;
  } content;
};

#endif // PI_THREAD_H
