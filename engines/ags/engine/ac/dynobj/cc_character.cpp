/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ags/engine/ac/dynobj/cc_character.h"
#include "ags/shared/ac/character_info.h"
#include "ags/engine/ac/global_character.h"
#include "ags/shared/ac/game_setup_struct.h"
#include "ags/shared/ac/game_version.h"
#include "ags/globals.h"

namespace AGS3 {



// return the type name of the object
const char *CCCharacter::GetType() {
	return "Character";
}

// serialize the object into BUFFER (which is BUFSIZE bytes)
// return number of bytes used
int CCCharacter::Serialize(const char *address, char *buffer, int bufsize) {
	const CharacterInfo *chaa = (const CharacterInfo *)address;
	StartSerialize(buffer);
	SerializeInt(chaa->index_id);
	return EndSerialize();
}

void CCCharacter::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &_GP(game).chars[num], this);
}

void CCCharacter::WriteInt16(const char *address, intptr_t offset, int16_t val) {
	*(int16_t *)(const_cast<char *>(address) + offset) = val;

	// Detect when a game directly modifies the inventory, which causes the displayed
	// and actual inventory to diverge since 2.70. Force an update of the displayed
	// inventory for older games that reply on the previous behaviour.
	if (_G(loaded_game_file_version) < kGameVersion_270) {
		const int invoffset = 112;
		if (offset >= invoffset && offset < (intptr_t)(invoffset + MAX_INV * sizeof(short))) {
			update_invorder();
		}
	}
}

} // namespace AGS3
