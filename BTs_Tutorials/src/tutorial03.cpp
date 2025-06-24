#include "dummy_nodes.h"
#include <behaviortree_cpp/bt_factory.h>

int main() {
  BT::BehaviorTreeFactory factory;
  
  factory.registerNodeType<CalculateGoal>("CalculateGoal");
  factory.registerNodeType<PrintTarget>("PrintTarget");

  static const char* xml_text = R"(
    <root BTCPP_format="4" >
        <BehaviorTree ID="MainTree">
          <Sequence name="root_sequence">
            <CalculateGoal goal="{GoalPosition}" />
            <PrintTarget   target="{GoalPosition}" />
            <Script        code=" OtherGoal:='-1;3' " />
            <PrintTarget   target="{OtherGoal}" />
          </Sequence>
        </BehaviorTree>
    </root>
  )";

  auto tree = factory.createTreeFromText(xml_text);
  
  tree.tickWhileRunning();
  
  return 0;
}