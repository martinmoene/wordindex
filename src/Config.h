/*
 * config.h - compiler dependent issues.
 *
 * This file is part of WordIndex.
 *
 * Copyright (C) 2007-2020, Martin J. Moene.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mngdriver; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id$
 */

/**
 * \file
 */

#ifndef config_h_included
#define config_h_included

/**
 * compiler selection.
 */
#if defined( _MSC_VER )
# define _WORDINDEX_MSC
# if _MSC_VER < 1310
#  define _WORDINDEX_MSC6
# endif

#elif defined ( __INTEL_COMPILER )
# define _WORDINDEX_INTEL

#elif defined ( __GNUC__ )
# define _WORDINDEX_GNU

#endif

/**
 * compiler dependent typename behaviour.
 */
#if defined( _WORDINDEX_MSC6 )
# define typename_type_k
#else
# define typename_type_k typename
#endif

#if defined ( _WORDINDEX_MSC6 )
namespace std {
template < typename T >
inline T min( T const& a, T const& b ) { return a < b ? a : b; }

template < typename T >
inline T max( T const& a, T const& b ) { return a > b ? a : b; }
} // namespace std
#endif

#endif // config_h_included

/*
 * end of file
 */
