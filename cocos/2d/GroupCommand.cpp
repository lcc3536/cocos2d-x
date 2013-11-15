//
// Created by NiTe Luo on 11/13/13.
//


#include "GroupCommand.h"

NS_CC_BEGIN

static GroupCommandManager* s_instance;
GroupCommandManager *GroupCommandManager::getInstance()
{
    if(!s_instance)
    {
        s_instance = new GroupCommandManager();
        if(!s_instance->init())
        {
            CC_SAFE_DELETE(s_instance);
        }
    }
    return s_instance;
}

GroupCommandManager::GroupCommandManager()
{

}

GroupCommandManager::~GroupCommandManager()
{
    CC_SAFE_RELEASE_NULL(s_instance);
}

bool GroupCommandManager::init()
{
    //0 is the default render group
    _groupMapping[0] = true;
    return true;
}

int GroupCommandManager::getGroupID()
{
    //Reuse old id
    for(auto it = _groupMapping.begin(); it != _groupMapping.end(); ++it)
    {
        if(!it->second)
        {
            return it->first;
        }
    }

    //Create new ID
    int newID = _groupMapping.size();
    _groupMapping[newID] = true;

    return newID;
}

void GroupCommandManager::releaseGroupID(int groupID)
{
    _groupMapping[groupID] = false;
}

GroupCommand::GroupCommand(int viewport, int32_t depth)
:RenderCommand()
, _viewport(viewport)
, _depth(depth)
{
    _type = GROUP_COMMAND;
    _renderQueueID = GroupCommandManager::getInstance()->getGroupID();
}

GroupCommand::~GroupCommand()
{
    GroupCommandManager::getInstance()->releaseGroupID(_renderQueueID);
}

int64_t GroupCommand::generateID()
{
    _id = 0;

    _id = (int64_t)_viewport << 61
            | (int64_t)1 << 60 // translucent
            | (int64_t)_depth << 36;

    return _id;
}


NS_CC_END
