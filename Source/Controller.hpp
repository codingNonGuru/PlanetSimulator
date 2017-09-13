#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

enum class Actions {THRUST, RETURN, BREAK, STEER_RIGHT, STEER_LEFT, SHOOT, COOL, MINE, COUNT};
enum class Keys {UP, RIGHT, LEFT, SPACE, COUNT};

class GameObject;
class Spaceship;

class Controller {
protected:
	GameObject* target_;
	Spaceship* parent_;
	bool actions_[(int)Actions::COUNT];
public:
	virtual void update() {}
	bool IsActing(Actions action);
	void SetAction(Actions, bool);
	void Initialize(Spaceship*);
	Controller() {}
	virtual ~Controller();
	virtual bool isHuman() const {return false;}
};

class HumanController : public Controller {
	void update() override;
	bool isHuman() const override {return true;}
};

class MachineController : public Controller {
	virtual void update() override;
	virtual bool isHuman() const override {return false;}
};

class BargeController : public MachineController
{
	void update() override;
};

#endif
