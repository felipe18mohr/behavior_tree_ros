#include "nodes.cpp"


int main(int argc, char** argv) {

  ros::init(argc, argv, "pick_and_place_tree");

  BT::BehaviorTreeFactory factory;
  // Registrando os Nodes da árvore
  factory.registerNodeType<MoveJointsActionNode>("MoveJoints");
  factory.registerNodeType<MoveToPoseActionNode>("MoveToPose");
  factory.registerNodeType<SetGripperActionNode>("SetGripper");
  
  // Criando a árvore, a partir do arquivo XML
  auto tree = factory.createTreeFromFile("src/behavior_tree_ros/baxter_bt/trees/pick_and_place_tree.xml");
  
  // Algumas funções de log
  BT::StdCoutLogger logger_cout(tree);
  BT::FileLogger logger_file(tree, "src/behavior_tree_ros/baxter_bt/loggers/bt_trace.fbl");
  BT::MinitraceLogger logger_minitrace(tree, "src/behavior_tree_ros/baxter_bt/loggers/bt_trace.json");
  BT::PublisherZMQ publisher_zmq(tree);

  BT::printTreeRecursively(tree.rootNode());

  // Continua executando um "tick" na Root enquanto não retornar SUCCESS ou FAILURE
  BT::NodeStatus status = BT::NodeStatus::RUNNING;
  while (status == BT::NodeStatus::RUNNING) {
    status = tree.tickRoot();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}
