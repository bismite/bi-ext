#include <bi/action.h>
#include <bi/context.h>

typedef struct {
  BiAction* action;
} ActionDoer;

void bi_action_start(BiNode *node, BiAction *action,double now)
{
  action->_start(node,action,now);
}

void bi_action_update(BiNode *node, BiAction *action, double rate)
{
  action->_update(node,action,rate);
}

static bool do_actions(BiContext* context,BiNode* node,double now,BiTimer* timer)
{
  ActionDoer* doer = timer->userdata;
  BiAction* a = doer->action;
  double rate = (now - a->start_at) / a->duration;
  if(rate<0.0) rate = 0.0;
  if(rate>1.0) rate = 1.0;
  bi_action_update(node,a,rate);
  return true;
}

BiTimer* bi_add_action(BiContext* context,BiNode* node,BiAction* action)
{
  BiTimer *timer = malloc(sizeof(BiTimer));
  bi_timer_init(timer, node, do_actions, 0, -1, NULL);
  ActionDoer* doer = malloc(sizeof(ActionDoer));
  doer->action = action;
  timer->userdata = doer;
  bi_add_timer(context,timer);
  return timer;
}
