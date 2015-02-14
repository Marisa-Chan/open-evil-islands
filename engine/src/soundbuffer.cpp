/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2015 Yanis Kurganov <ykurganov@users.sourceforge.net>
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

#include "soundbuffer.hpp"

namespace cursedearth
{
    sound_buffer_t::sound_buffer_t(const sound_format_t& format):
        m_format(format)
    {
    }

    void sound_buffer_t::write(const sound_block_ptr_t& block)
    {
        m_buffer.push(block);
    }

    sound_block_ptr_t sound_buffer_t::read()
    {
        if (!m_current_block) {
            m_buffer.pop(m_current_block);
        }
        sound_block_ptr_t block = m_current_block;
        m_current_block.reset();
        return block;
    }

    bool sound_buffer_t::try_read_one_sample(uint8_t data[SOUND_CAPABILITY_MAX_SAMPLE_SIZE])
    {
        if (!m_current_block) {
            if (!m_buffer.pop(m_current_block, false)) {
                return false;
            }
        }

        const size_t size = m_current_block->read(data, m_format.sample_size);
        if (0 == size) {
            release_block(m_current_block);
            m_current_block.reset();
            return try_read_one_sample(data);
        }

        assert(size == m_format.sample_size);
        return true;
    }

    sound_block_ptr_t sound_buffer_t::acquire_block()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ignore = lock;

        if (m_blocks.empty()) {
            return std::make_shared<sound_block_t>(m_format);
        }

        sound_block_ptr_t block = m_blocks.back();
        m_blocks.pop_back();

        block->reset();
        return block;
    }

    void sound_buffer_t::release_block(const sound_block_ptr_t& block)
    {
        assert(m_format == block->format());
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ignore = lock;
        m_blocks.push_back(block);
    }
}
