#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

enum class Actions {THRUST, RETURN, BREAK, STEER_RIGHT, STEER_LEFT, SHOOT, COOL, MINE, UNLOAD, COUNT};
enum class Missions {MINE, DELIVER, COUNT};
enum class Keys {UP, RIGHT, LEFT, SPACE, COUNT};

class GameObject;
class Spaceship;

class Controller {
protected:
	GameObject* target_;
	Spaceship* parent_;
	bool actions_[(int)Actions::COUNT];
	bool missions_[(int)Missions::COUNT];
public:
	virtual void update() {}
	bool IsActing(Actions action);
	void SetAction(Actions, bool);
	bool GetMission(Missions);
	void SetMission(Missions, bool);
	void Initialize(Spaceship*);
	Controller() {}
	virtual ~Controller();
	virtual bool isHuman() const {return false;}
	void ReachTarget(float, float);
	void ComputeDistance();
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
