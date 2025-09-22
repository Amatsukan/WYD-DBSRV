#pragma once
#include "../common.h"
class ConfigManager {
public:
bool load(const std::string& filename);
void save(const std::string& filename);
int getSapphire() const { return m_sapphire; }
unsigned short getLastCapsule() const { return m_lastCapsule; }

void setSapphire(int value) { m_sapphire = value; }
void setLastCapsule(unsigned short value) { m_lastCapsule = value; }


private:
int m_sapphire = 8;
unsigned short m_lastCapsule = 0;
};