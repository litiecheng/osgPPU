/***************************************************************************
 *   Copyright (c) 2008   Art Tevs                                         *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 3 of        *
 *   the License, or (at your option) any later version.                   *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesse General Public License for more details.                    *
 *                                                                         *
 *   The full license is in LICENSE file included with this distribution.  *
 ***************************************************************************/

#include <osgPPU/Pipeline.h>

namespace osgPPU
{

//------------------------------------------------------------------------------
Pipeline::Pipeline() : osgUtil::RenderBin()
{
}

//------------------------------------------------------------------------------
Pipeline::Pipeline(const Pipeline& pipeline, const osg::CopyOp& copyop) :
    osgUtil::RenderBin(pipeline, copyop),
    mName(pipeline.mName)
{

}

//------------------------------------------------------------------------------
Pipeline::~Pipeline()
{

}


//------------------------------------------------------------------------------
struct Pipeline::SortByIndex : std::less<osgUtil::RenderLeaf*>
{
    public:
        bool operator() (const osgUtil::RenderLeaf* lhs,const osgUtil::RenderLeaf* rhs) const
        {
            // both renderleafs must be of the type unit
            const osg::Drawable* a = dynamic_cast<const osg::Drawable*>(lhs->getDrawable());
            const osg::Drawable* b = dynamic_cast<const osg::Drawable*>(rhs->getDrawable());

            // sort by operator
            if (a && b)
            {
                const Unit::DrawCallback* adc = dynamic_cast<const Unit::DrawCallback*>(a->getDrawCallback());
                const Unit::DrawCallback* bdc = dynamic_cast<const Unit::DrawCallback*>(b->getDrawCallback());

                if (adc && bdc)
                    return (*(adc->getParent()) < *(bdc->getParent()));
            }

            // give some warning
            osg::notify(osg::WARN) << "osgPPU::Pipeline::SortByIndex pipeline contains drawables of non-valid type. Cannot handle them!" << std::endl;
            return false;
        }
};

//------------------------------------------------------------------------------
void Pipeline::sortImplementation()
{
    copyLeavesFromStateGraphListToRenderLeafList();

    // now sort the list into acending depth order.
    std::sort(_renderLeafList.begin(),_renderLeafList.end(), SortByIndex());
}


}; //end namespace




