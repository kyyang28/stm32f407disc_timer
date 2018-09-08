
#include "feature.h"
#include "configMaster.h"

static uint32_t activeFeaturesLatch = 0;

void intFeatureClearAll(uint32_t *features)
{
	*features = 0;
}

void intFeatureSet(uint32_t mask, uint32_t *features)
{
	*features |= mask;
}

void intFeatureClear(uint32_t mask, uint32_t *features)
{
	*features &= ~(mask);
}

void latchActiveFeatures(void)
{
	activeFeaturesLatch = masterConfig.enabledFeatures;
}

bool featureConfigured(uint32_t mask)
{
	return masterConfig.enabledFeatures & mask;
}

bool feature(uint32_t mask)
{
	return activeFeaturesLatch & mask;
}

void featureSet(uint32_t mask)
{
	intFeatureSet(mask, &masterConfig.enabledFeatures);
}

void featureClear(uint32_t mask)
{
	intFeatureClear(mask, &masterConfig.enabledFeatures);
}

void featureClearAll(void)
{
	intFeatureClearAll(&masterConfig.enabledFeatures);
}

uint32_t featureMask(void)
{
	return masterConfig.enabledFeatures;
}
