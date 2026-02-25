#pragma once

#define REGISTER_APP_INSTANCE(APPCLASS, VAR) \
    App* VAR = new APPCLASS(); \
    static struct VAR##_ref { VAR##_ref() { (void)VAR; } } VAR##_ref_instance;

// std lib
#include <cstdint>
#include <mutex>

class App {
private:
	static constexpr uint32_t s_kMaxApps = 6;
	static App* s_apps[s_kMaxApps];
	static std::mutex s_mtx;

protected:
	static bool registerApp(uint32_t _index, App* _app);

public:
	App() = default;
	virtual ~App() = 0;

	virtual bool init() = 0;
	virtual void draw(double _dt) = 0;
	virtual void shutdown() = 0;

	static App* getApp(uint32_t _index);

};

