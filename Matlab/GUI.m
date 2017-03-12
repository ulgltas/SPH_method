%**************************************************************************
% GUI: Pseudo Graphical Interface
%                 - Compile the project
%                 - Run an experiment
%                 - Analyse an experiment
%**************************************************************************
clc; clear all; close all;

% Get Environement:
name = getenv('COMPUTERNAME');
if (strcmp(name,'DESKTOP-31TT348')) 
     path = 'C:\Users\gabri\Dropbox\Applications\ShareLaTeX\SPH_PROJECT\';
     disp(['Welcome GabyGab: ']); disp(['*****************']);disp([' ']);
else path = ''; 
end

loop=1;

while (loop==1)
% Choices
disp(['Would you like to:']);
disp(['    1) Compile the project; ']);
disp(['    2) Run an experiment; ']);
disp(['    3) Analyse an experiment; ']);
disp(['    4) Edit a new experiment; ']);
disp(['    5) Exit; ', ' ']);
choice = input('Enter your choice number: ' ); disp(' ');

switch(choice)
    case 1 % Compile:
        cd ..; cd build\;
        if(ispc) system('mingw32-make')
        elseif(isunix) system('make')
        else disp(['You are not allowed to compile... ']);
        end
        cd ..; cd Matlab\;
        
    case 2 % Run:
        if(strcmp(name,'DESKTOP-31TT348'))
           disp(['Choose among the list of Playgrounds: ']); dir('..\Playgrounds')
           p = input('Parameter file name (w/ .kzr): ','s'); p = strcat(p, '.kzr ');
           g = input('Geometry file name (w/ .kzr): ','s'); g = strcat(g, '.kzr ');
           system(char(strcat({'"../build/sph.exe"'},{' '},p,{' '},g,{' '}, {'FreeFallingCube'})))
        else disp(['You are not allowed to launch an experiment... ']);
        end

    case 3 % Analyse:
        disp(['Experiments: ']); % Display Possibilities
        disp(['1) Free Falling Cube']);
        disp(['2) Free Falling Cube with random particules']);
        disp(['3) Stationary Tank']);disp([' ']);
        n = input('Enter the number of the experiment: ');
        exit = TestCases(n, path)
    
    case 4 % Edit File
        cd ..; cd Playgrounds\;
        p = input('NEW Parameter file name: ','s'); p = strcat(p, '.kzr ');
        if (exist(p, 'file')==0) 
            fid = fopen( p, 'wt' );
            edit(p);
        end
        g = input('NEW Geometry file name: ','s'); g = strcat(g, '.kzr ');
        if (exist(g, 'file')==0) 
            fid = fopen( g, 'wt' );
            edit(g);
        end
        input('Enter: '); fclose(p); fclose(g);
        cd ..; cd Matlab\;
        
    case 5 % Exit
        loop=0;
    otherwise disp(['Not valid choice']); disp(' ');
end

end





