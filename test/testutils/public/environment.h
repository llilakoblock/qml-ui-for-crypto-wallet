#pragma once

#include <gtest/gtest.h>

namespace freedom
{
namespace testutils
{

class Environment : public ::testing::Environment
{
public:
	virtual ~Environment() override = default;

public:
	virtual void SetUp() override;
	virtual void TearDown() override;
};

} // namespace testutils
} // namespace freedom
