#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

enum class Actions {THRUST, BREAK, STEER_RIGHT, STEER_LEFT, SHOOT, MINE, COUNT};
enum class Keys {UP, RIGHT, LEFT, SPACE, COUNT};

class Controller {
protected:
	bool actions_[(int)Actions::COUNT];
public:
	virtual void update() {}
	bool isActing(Actions action);
	Controller() {}
	virtual ~Controller();
	virtual bool isHuman() const {return false;}
};

class HumanController : public Controller {
	void update() override;
	bool isHuman() const override {return true;}
};

class MachineController : public Controller {
	bool isHuman() const override {return false;}
};

#endif
