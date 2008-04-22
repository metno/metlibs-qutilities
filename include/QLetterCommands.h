/*
  libqUtilities - Diverse Qt-classes and coserver base
  
  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _QLetterCommands_h
#define _QLetterCommands_h

#include <puTools/miString.h>

namespace qmstrings{
  const miString vprof               = "vprof";
  const miString addimage            = "addimage";
  const miString positions           = "positions";
  const miString showpositions       = "showpositions";
  const miString hidepositions       = "hidepositions";
  const miString changeimage         = "changeimage";
  const miString showpositionname    = "showpositionname";
  const miString showpositiontext    = "showpositiontext";
  const miString enableposclick      = "enableposclick";
  const miString enableposmove       = "enableposmove";
  const miString showtext            = "showtext";
  const miString enableshowtext      = "enableshowtext";
  const miString textrequest         = "textrequest";
  const miString selectposition     = "selectposition";
  const miString settime             = "settime";
  const miString remove              = "remove";
  const miString removeclient        = "removeclient";
  const miString newclient           = "newclient";
  const miString allclients          = "allclients";
  const miString timechanged         = "timechanged";
  const miString init_HQC_params     = "init_HQC_params";
  const miString update_HQC_params   = "update_HQC_params";
  const miString select_HQC_param    = "select_HQC_param";
  const miString apply_quickmenu     = "apply_quickmenu";
  const miString station             = "station";
  const miString changeimageandtext  = "change_image_and_text";
  const miString changeimageandimage = "change_image_and_image";
  const miString seteditpositions    = "seteditpositions";
  const miString editposition        = "editposition";
  const miString copyvalue           = "copyvalue";
  const miString areas               = "areas";
  const miString selectarea          = "selectarea";
  const miString areacommand         = "areacommand";
  const miString showarea            = "showarea";
  const miString changearea          = "changearea";
  const miString deletearea          = "deletearea";
  const miString annotation          = "annotation";
  const miString changetype          = "changetype";
  const miString autoredraw          = "autoredraw";
  const miString redraw              = "redraw";
  const miString sendkey             = "sendkey";
  const miString editmode            = "editmode";
  const miString printclicked        = "printclicked";

  const int default_id = -1000;
  const int all = -1;
  const int port = 19444;
};
#endif


