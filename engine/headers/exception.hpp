/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2017 Yanis Kurganov <ykurganov@users.sourceforge.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CE_EXCEPTION_HPP
#define CE_EXCEPTION_HPP

#include <stdexcept>

#include <boost/format.hpp>

namespace cursedearth
{
    class game_error: public std::runtime_error
    {
        template <typename T>
        void apply_cascade(boost::format& format, T arg)
        {
            format % arg;
        }

        template <typename T, typename... A>
        void apply_cascade(boost::format& format, T arg, A&&... args)
        {
            format % arg;
            apply_cascade(format, std::forward<A>(args)...);
        }

        template <typename... A>
        boost::format make_format(const std::string& message, A&&... args)
        {
            boost::format format(message);
            apply_cascade(format, std::forward<A>(args)...);
            return format;
        }

    public:
        game_error(const std::string& token, const std::string& message):
            std::runtime_error(token + ": " + message) {}

        game_error(const std::string& token, const boost::format& format):
            game_error(token, str(format)) {}

        template <typename... A>
        game_error(const std::string& token, const std::string& message, A&&... args):
            game_error(token, make_format(message, std::forward<A>(args)...)) {}
    };
}

#endif
