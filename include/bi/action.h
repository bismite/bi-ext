#ifndef __BI_EXT_ACTION_H__
#define __BI_EXT_ACTION_H__

#include <bi/node.h>
#include <bi/timer.h>

typedef struct _BiAction BiAction;

typedef void (*bi_action_start_function)(BiNode*,BiAction*,double); // target-node,action,now
typedef bool (*bi_action_update_function)(BiNode*,BiAction*,double); // target-node,action,rate

struct _BiAction {
  bi_action_start_function _start;
  bi_action_update_function _update;
  bool finit;
  double start_at;
  double duration;
  void* action_data;
};

extern void bi_action_start(BiNode *node, BiAction *action,double now);
extern void bi_action_update(BiNode *node, BiAction *action, double rate);

extern BiTimer* bi_add_action(BiContext* context,BiNode* node,BiAction* action);

//
// actions
//
extern void bi_action_move_to_init(BiAction* action,double duration,int x,int y);
extern void bi_action_sequence_init(BiAction* action,size_t num,BiAction** actions);
extern void bi_action_repeat_init(BiAction* action,BiAction* target);

#endif
