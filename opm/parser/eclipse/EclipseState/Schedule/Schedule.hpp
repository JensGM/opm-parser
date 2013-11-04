/*
  Copyright 2013 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <opm/parser/eclipse/Deck/Deck.hpp>

#include <opm/parser/eclipse/EclipseState/Schedule/TimeMap.hpp>


namespace Opm 
{
    
    const boost::gregorian::date defaultStartDate( 1983 , boost::gregorian::Jan , 1);

    class Schedule {
    public:
        Schedule(DeckConstPtr deck);
        boost::gregorian::date getStartDate() const;
        TimeMapConstPtr getTimeMap() const;
    private:
        TimeMapPtr m_timeMap;
    };
    typedef boost::shared_ptr<Schedule> SchedulePtr;
    typedef boost::shared_ptr<const Schedule> ScheduleConstPtr;
}

#endif