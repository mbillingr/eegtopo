
% Usage:
%  TOPOPLOT( FILENAME, DATASTRUCT )
%  TOPOPLOT( FILENAME, LABELS, VALUES )


%
%  This file is part of EEGtopo.
%
%  Foobar is free software: you can redistribute it and/or modify
%  it under the terms of the GNU General Public License as published by
%  the Free Software Foundation, either version 3 of the License, or
%  (at your option) any later version.
%
%  Foobar is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%  GNU General Public License for more details.
%
%  You should have received a copy of the GNU General Public License
%  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
%
%
%  Copyright 2012 Martin Billinger
%
%

function eeg_topoplot( filename, channels, values )

    if nargin == 2
        datastruct = channels;
        channels = fieldnames( datastruct );
        for i = 1 : length(channels);
            values(i) = datastruct.(channels{i});
        end
    end

    for i = 1 : length(channels);
        if strcmpi(channels{i},'I1'), channels{i} = 'O9'; end
        if strcmpi(channels{i},'I2'), channels{i} = 'O10'; end
    end

    scriptfile = [filename '.txt'];
    mpath = fileparts(mfilename('fullpath'));
    executable = 'eegtopo';

    if ~exist( [mpath filesep executable], 'file' )
        error( [executable ' executable must be in the same directory as ' mfilename '.m (' mpath '/).'] );
    end

    f = fopen( scriptfile, 'w' );

    chstr = sprintf( '%s,', channels{:} );
    chstr(end) = [];

    fprintf( f, 'define "layout": %s;\n', chstr );
    fprintf( f, 'set "layout".visible = true;\n' );
    fprintf( f, 'set "layout".show_label = false;\n' );
    fprintf( f, 'set "layout".line_width = 0;\n' );
    fprintf( f, 'set "layout".fill_color = [0,0,0];\n' );
    fprintf( f, 'set "layout".radius = 0.02;\n' );
    fprintf( f, 'set "layout".value = 0.0;\n' );

    for i = 1 : length(channels)
        fprintf( f, 'set %s.value = %f;\n', channels{i}, values(i) );
    end

    fprintf( f, 'draw_topo;\n' );
    fprintf( f, 'draw_head;\n' );
    fprintf( f, 'draw_electrodes;\n' );

    fclose( f );

    system( [mpath filesep executable ' -f' scriptfile ' -o' filename] );

    delete( scriptfile );

end
