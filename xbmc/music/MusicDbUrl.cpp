/*
 *      Copyright (C) 2012 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MusicDbUrl.h"
#include "filesystem/MusicDatabaseDirectory.h"
#include "utils/StringUtils.h"
#include "utils/Variant.h"

using namespace std;
using namespace XFILE;
using namespace XFILE::MUSICDATABASEDIRECTORY;

CMusicDbUrl::CMusicDbUrl()
  : CDbUrl()
{ }

CMusicDbUrl::~CMusicDbUrl()
{ }

bool CMusicDbUrl::parse()
{
  // the URL must start with musicdb://
  if (m_url.GetProtocol() != "musicdb" || m_url.GetFileName().empty())
    return false;

  CStdString path = m_url.Get();
  NODE_TYPE dirType = CMusicDatabaseDirectory::GetDirectoryType(path);
  NODE_TYPE childType = CMusicDatabaseDirectory::GetDirectoryChildType(path);

  switch (dirType)
  {
    case NODE_TYPE_ARTIST:
      m_type = "artists";
      break;
      
    case NODE_TYPE_ALBUM:
    case NODE_TYPE_ALBUM_RECENTLY_ADDED:
    case NODE_TYPE_ALBUM_RECENTLY_PLAYED:
    case NODE_TYPE_ALBUM_TOP100:
    case NODE_TYPE_ALBUM_COMPILATIONS:
    case NODE_TYPE_YEAR_ALBUM:
    case NODE_TYPE_SINGLES:
      m_type = "albums";
      break;

    case NODE_TYPE_ALBUM_RECENTLY_ADDED_SONGS:
    case NODE_TYPE_ALBUM_RECENTLY_PLAYED_SONGS:
    case NODE_TYPE_ALBUM_TOP100_SONGS:
    case NODE_TYPE_ALBUM_COMPILATIONS_SONGS:
    case NODE_TYPE_SONG:
    case NODE_TYPE_SONG_TOP100:
    case NODE_TYPE_YEAR_SONG:
      m_type = "songs";
      break;

    default:
      break;
  }

  switch (childType)
  {
    case NODE_TYPE_ARTIST:
      m_type = "artists";
      break;
      
    case NODE_TYPE_ALBUM:
    case NODE_TYPE_ALBUM_RECENTLY_ADDED:
    case NODE_TYPE_ALBUM_RECENTLY_PLAYED:
    case NODE_TYPE_ALBUM_TOP100:
    case NODE_TYPE_YEAR_ALBUM:
      m_type = "albums";
      break;
      
    case NODE_TYPE_SONG:
    case NODE_TYPE_ALBUM_RECENTLY_ADDED_SONGS:
    case NODE_TYPE_ALBUM_RECENTLY_PLAYED_SONGS:
    case NODE_TYPE_ALBUM_TOP100_SONGS:
    case NODE_TYPE_ALBUM_COMPILATIONS_SONGS:
    case NODE_TYPE_SONG_TOP100:
    case NODE_TYPE_YEAR_SONG:
      m_type = "songs";
      break;

    case NODE_TYPE_GENRE:
      m_type = "genres";
      break;
      
    case NODE_TYPE_YEAR:
      m_type = "years";
      break;
      
    case NODE_TYPE_ALBUM_COMPILATIONS:
      m_type = "albums";
      break;
      
    case NODE_TYPE_SINGLES:
      m_type = "albums";
      break;

    case NODE_TYPE_TOP100:
      m_type = "top100";
      break;
      
    case NODE_TYPE_ROOT:
    case NODE_TYPE_OVERVIEW:
    default:
      return false;
  }

  if (m_type.empty())
    return false;

  // parse query params
  CQueryParams queryParams;
  CDirectoryNode::GetDatabaseInfo(path, queryParams);

  // retrieve and parse all options
  AddOptions(m_url.GetOptions());

  // add options based on the QueryParams
  if (queryParams.GetArtistId() != -1)
    AddOption("artistid", queryParams.GetArtistId());
  if (queryParams.GetAlbumId() != -1)
    AddOption("albumid", queryParams.GetAlbumId());
  if (queryParams.GetGenreId() != -1)
    AddOption("genreid", queryParams.GetGenreId());
  if (queryParams.GetSongId() != -1)
    AddOption("songid", queryParams.GetSongId());
  if (queryParams.GetYear() != -1)
    AddOption("year", queryParams.GetYear());

  return true;
}
