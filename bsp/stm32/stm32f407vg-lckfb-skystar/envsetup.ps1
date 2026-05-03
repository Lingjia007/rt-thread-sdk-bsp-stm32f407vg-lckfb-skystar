function Show-Logo {
    $macaronColors = @(
        "$([char]27)[38;2;80;200;160m",
        "$([char]27)[38;2;100;210;220m",
        "$([char]27)[38;2;240;220;60m",
        "$([char]27)[38;2;255;180;50m",
        "$([char]27)[38;2;255;100;80m",
        "$([char]27)[38;2;255;80;150m"
    )
    $reset = "$([char]27)[0m"
    $logo = @(
        '     ____  ______________                        __ ',
        '    / __ \/_  __/_  __/ /_  ________  ____ _____/ / ',
        '   / /_/ / / /   / / / __ \/ ___/ _ \/ __ `/ __  / ',
        '  / _, _/ / /   / / / / / / /  /  __/ /_/ / /_/ /  ',
        ' /_/ |_| /_/   /_/ /_/ /_/_/   \___/\__,_/\__,_/   '
    )
    Write-Host ''
    foreach ($line in $logo) {
        $i = 0
        foreach ($char in $line.ToCharArray()) {
            $color = $macaronColors[$i % $macaronColors.Count]
            Write-Host "$color$char" -NoNewline
            $i++
        }
        Write-Host $reset
    }
    Write-Host ''
    Write-Host ' RT-Thread Development Environment by Lingsir007' -ForegroundColor White -BackgroundColor DarkBlue
    Write-Host ''
}

function Show-Info {
    Write-Host ' Project: ' -NoNewline -ForegroundColor White
    Write-Host $env:SDK_PRJ_TOP_DIR -ForegroundColor Cyan
    Write-Host ' ENV_ROOT: ' -NoNewline -ForegroundColor White
    Write-Host $env:ENV_ROOT -ForegroundColor Cyan
    Write-Host ' GCC: ' -NoNewline -ForegroundColor White
    Write-Host $env:GCCPath -ForegroundColor Cyan
    Write-Host ''
}

function Test-Dependencies {
    Write-Host ' Checking dependencies...' -ForegroundColor Yellow
    & "$env:PythonPath\python.exe" -c 'import scons; import kconfiglib; import curses; import requests; import tqdm' 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) {
        Write-Host ' [INSTALL] Installing required packages...' -ForegroundColor Yellow
        & "$env:PythonPath\python.exe" -m pip install scons kconfiglib windows-curses requests tqdm -q
        if ($LASTEXITCODE -ne 0) {
            Write-Host ' [ERROR] Failed to install dependencies!' -ForegroundColor Red
            Read-Host 'Press Enter to exit'
            exit 1
        }
        Write-Host ' [OK] Dependencies installed' -ForegroundColor Green
    } else {
        Write-Host ' [OK] All dependencies installed' -ForegroundColor Green
    }
}

function Show-Commands {
    Write-Host ''
    Write-Host "$([char]27)[48;5;166m Commands:$([char]27)[0m"
    Write-Host ''
    Write-Host '   menu   ' -NoNewline -ForegroundColor Cyan
    Write-Host ' - Open menuconfig' -ForegroundColor White
    Write-Host '   m      ' -NoNewline -ForegroundColor Green
    Write-Host ' - Build project (scons -j64)' -ForegroundColor White
    Write-Host '   mc     ' -NoNewline -ForegroundColor Yellow
    Write-Host ' - Clean project' -ForegroundColor White
    Write-Host '   pkgs   ' -NoNewline -ForegroundColor Magenta
    Write-Host ' - Update packages' -ForegroundColor White
    Write-Host '   pkgu   ' -NoNewline -ForegroundColor Blue
    Write-Host ' - Upgrade packages' -ForegroundColor White
    Write-Host '   mdk5   ' -NoNewline -ForegroundColor Red
    Write-Host ' - Generate & Build MDK5 project' -ForegroundColor White
    Write-Host ''
}

function global:Write-RainbowLine {
    param([string]$Line)
    
    $colors = @('Red', 'DarkYellow', 'Yellow', 'Green', 'Cyan', 'Blue', 'Magenta')
    $words = $Line -split '(\s+)'
    $i = 0
    foreach ($word in $words) {
        if ($word -match '^\s+$') {
            Write-Host $word -NoNewline
        }
        elseif ($word) {
            $color = $colors[$i % $colors.Count]
            Write-Host $word -NoNewline -ForegroundColor $color
            $i++
        }
    }
    Write-Host ''
}

function global:Write-ColoredSconsOutput {
    param([string]$Line)
    
    if ($Line -match '^(\s*)(CC|CXX|AS|AR|LD|LINK|OBJCOPY|OBJDUMP|SIZE|STRIP)\s+(.+)$') {
        $prefix = $Matches[1]
        $action = $Matches[2]
        $target = $Matches[3]
        Write-Host $prefix -NoNewline
        Write-Host $action -NoNewline -ForegroundColor Cyan
        Write-Host " $target"
    }
    elseif ($Line -match '^(\s*)(Removed|Deleting)\s+(.+)$') {
        $prefix = $Matches[1]
        $action = $Matches[2]
        $target = $Matches[3]
        Write-Host $prefix -NoNewline
        Write-Host $action -NoNewline -ForegroundColor Yellow
        Write-Host " $target"
    }
    elseif ($Line -match '^(\s*)(compiling|assembling|linking)\s+(.+)$') {
        $prefix = $Matches[1]
        $action = $Matches[2]
        $target = $Matches[3]
        Write-Host $prefix -NoNewline
        Write-Host $action -NoNewline -ForegroundColor Cyan
        Write-Host " $target"
    }
    elseif ($Line -match '^(\s*)(linking)\.\.\.') {
        $prefix = $Matches[1]
        Write-Host "${prefix}linking..." -ForegroundColor Cyan
    }
    elseif ($Line -match '^(\s*)(Building|Generating)\s+(.+)$') {
        $prefix = $Matches[1]
        $action = $Matches[2]
        $target = $Matches[3]
        Write-Host $prefix -NoNewline
        Write-Host $action -NoNewline -ForegroundColor Magenta
        Write-Host " $target"
    }
    elseif ($Line -match '^Program Size:') {
        Write-RainbowLine $Line
    }
    elseif ($Line -match '^Memory region') {
        Write-RainbowLine $Line
    }
    elseif ($Line -match '^\s+(CODE|RAM1|RAM2|FLASH|ROM):') {
        Write-RainbowLine $Line
    }
    elseif ($Line -match '^Build Time Elapsed:') {
        Write-Host $Line -ForegroundColor Cyan
    }
    elseif ($Line -match 'After Build') {
        Write-Host $Line -ForegroundColor DarkCyan
    }
    elseif ($Line -match '\d+ Error\(s\), \d+ Warning\(s\)') {
        if ($Line -match '^[1-9]\d* Error') {
            Write-Host $Line -ForegroundColor Red
        }
        elseif ($Line -match '^[1-9]\d* Warning') {
            Write-Host $Line -ForegroundColor Yellow
        }
        else {
            Write-Host $Line -ForegroundColor Green
        }
    }
    elseif ($Line -match 'has generated successfully') {
        Write-Host $Line -ForegroundColor Green
    }
    elseif ($Line -match 'Keil Version:|target_name:|Start to build') {
        Write-Host $Line -ForegroundColor DarkCyan
    }
    elseif ($Line -match '^\s*\d+\s*\|') {
        Write-Host $Line -ForegroundColor White
    }
    elseif ($Line -match 'warning:') {
        Write-Host $Line -ForegroundColor Yellow
    }
    elseif ($Line -match '^scons:') {
        if ($Line -match 'error|Error|ERROR') {
            Write-Host $Line -ForegroundColor Red
        }
        elseif ($Line -match 'warning|Warning|WARNING') {
            Write-Host $Line -ForegroundColor Yellow
        }
        elseif ($Line -match 'done|Building|Reading|Cleaning') {
            Write-Host $Line -ForegroundColor Cyan
        }
        else {
            Write-Host $Line -ForegroundColor White
        }
    }
    elseif ($Line -match '^\*\*\*') {
        Write-Host $Line -ForegroundColor Cyan
    }
    elseif ($Line -match 'error|Error|ERROR') {
        Write-Host $Line -ForegroundColor Red
    }
    elseif ($Line -match 'warning|Warning|WARNING') {
        Write-Host $Line -ForegroundColor Yellow
    }
    else {
        Write-Host $Line
    }
}

function global:Invoke-SconsColored {
    param([string[]]$SconsArgs)
    
    $outFile = Join-Path $env:TEMP 'scons_out.txt'
    $errFile = Join-Path $env:TEMP 'scons_err.txt'
    
    if (Test-Path $outFile) { Remove-Item $outFile -Force }
    if (Test-Path $errFile) { Remove-Item $errFile -Force }
    
    $process = Start-Process -FilePath "$env:PythonPath\python.exe" `
        -ArgumentList (@('-m', 'SCons') + $SconsArgs) `
        -RedirectStandardOutput $outFile `
        -RedirectStandardError $errFile `
        -NoNewWindow `
        -PassThru
    
    $lastLineCount = 0
    while (-not $process.HasExited) {
        if (Test-Path $outFile) {
            $lines = Get-Content $outFile -ErrorAction SilentlyContinue
            if ($lines) {
                $newLines = $lines | Select-Object -Skip $lastLineCount
                foreach ($line in $newLines) {
                    Write-ColoredSconsOutput $line
                }
                $lastLineCount = $lines.Count
            }
        }
        Start-Sleep -Milliseconds 100
    }
    
    if (Test-Path $outFile) {
        $lines = Get-Content $outFile -ErrorAction SilentlyContinue
        if ($lines) {
            $newLines = $lines | Select-Object -Skip $lastLineCount
            foreach ($line in $newLines) {
                Write-ColoredSconsOutput $line
            }
        }
    }
    
    if (Test-Path $errFile) {
        $errors = Get-Content $errFile -ErrorAction SilentlyContinue
        if ($errors) {
            $errors | ForEach-Object { Write-Host $_ -ForegroundColor Red }
        }
    }
    
    return $process.ExitCode
}

function global:menu {
    & "$env:PythonPath\python.exe" -m SCons --menuconfig
}

function global:m {
    Invoke-SconsColored -SconsArgs @('-j64') @args
}

function global:mc {
    Invoke-SconsColored -SconsArgs @('-c')
}

function global:pkgs {
    & "$env:PythonPath\python.exe" "$env:ENV_ROOT\tools\scripts\env.py" pkg --update
}

function global:pkgu {
    & "$env:PythonPath\python.exe" "$env:ENV_ROOT\tools\scripts\env.py" pkg --upgrade
}

function global:mdk5 {
    Invoke-SconsColored -SconsArgs @('--target=mdk5')
}

Show-Logo
Show-Info
Test-Dependencies
Show-Commands
