/*
 Copyright 2013 Adobe Systems Incorporated
 Distributed under the terms of the Gnu General Public License
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// Authors: Nicholas J. Bryan

#include "Layer.h"


void Layer::setYRange( Range<double> rY_, double maxY_)
{
    rY = rY_;
    maxY = maxY_;
}
void Layer::setXRange( Range<double> rX_, double maxX_)
{
    rX = rX_;
    maxX = maxX_;
}
Range<double> Layer::getYRange()
{
    return rY;
}
Range<double> Layer::getXRange()
{
    return rX;
}
double Layer::getMaxX()
{
    return maxX;
}
double Layer::getMaxY()
{
    return maxY;
}