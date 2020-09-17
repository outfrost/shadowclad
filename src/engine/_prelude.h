/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_PRELUDE_H_
#define ENGINE_PRELUDE_H_

#ifdef __GNUC__

#	define UNUSED __attribute__((unused))

#else // __GNUC__

#	define UNUSED

#endif // __GNUC__

#endif // ENGINE_PRELUDE_H_
