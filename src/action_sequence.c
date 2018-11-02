#include <bi/ext/action.h>
#include <stdlib.h>

static bool bi_action_sequence_update(BiNode* node, BiAction* action, double rate)
{
  double now = action->start_at + action->duration * rate;
  BiActionSequence* seq = action->action_data;
  double start_at= action->start_at;
  double end_at  = start_at;
  BiAction* current_action = NULL;
  double current_action_rate = 0;

  // search current action
  for(int i=0;i<seq->actions_size;i++) {
    end_at += seq->actions[i]->duration;
    if( start_at <= now && now <= end_at ) {
      // found!
      current_action = seq->actions[i];
      current_action_rate = (now-current_action->start_at) / current_action->duration;
      break;
    }
    start_at = end_at;
  }

  // all action done.
  if( current_action == NULL ) {
    return true;
  }

  // start new action
  if(seq->current_action != current_action) {
    if(seq->current_action) {
      // finish old action
      bi_action_update( node, seq->current_action, 1.0 );
    }
    // start new action
    bi_action_start(node,current_action,now);
    seq->current_action = current_action;
    current_action_rate = 0;
  }

  // do action
  bi_action_update( node, current_action, current_action_rate );

  return true;
}

static void bi_action_sequence_start(BiNode* node, BiAction* action,double now)
{
  BiActionSequence* seq = action->action_data;
  action->start_at = now;
  seq->current_action = NULL;
}

void bi_action_sequence_init(BiAction* action,size_t num,BiAction** actions)
{
  BiActionSequence* seq = action->action_data;
  seq->current_action = NULL;
  seq->actions_size = num;
  for(int i=0;i<num;i++) {
    seq->actions[i] = actions[i];
    action->duration += actions[i]->duration;
  }
  action->update = bi_action_sequence_update;
  action->start = bi_action_sequence_start;
  action->action_data = seq;
}
