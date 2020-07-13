.. # Copyright (C) 2017-2018, 2020  Alain Martin
.. #
.. # This file is part of FRUT.
.. #
.. # FRUT is free software: you can redistribute it and/or modify
.. # it under the terms of the GNU General Public License as published by
.. # the Free Software Foundation, either version 3 of the License, or
.. # (at your option) any later version.
.. #
.. # FRUT is distributed in the hope that it will be useful,
.. # but WITHOUT ANY WARRANTY; without even the implied warranty of
.. # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
.. # GNU General Public License for more details.
.. #
.. # You should have received a copy of the GNU General Public License
.. # along with FRUT.  If not, see <http://www.gnu.org/licenses/>.

jucer_project_begin
===================

Indicate the beginning of a JUCE project.

::

  jucer_project_begin(
    [JUCER_FORMAT_VERSION <jucer_format_version>]
    [JUCER_VERSION <jucer_version>]
    [PROJECT_FILE <jucer_file_path>]
    [PROJECT_ID <jucer_project_id>]
  )

You should call this command before any other :ref:`jucer_* command
<Reprojucer-commands>`.
