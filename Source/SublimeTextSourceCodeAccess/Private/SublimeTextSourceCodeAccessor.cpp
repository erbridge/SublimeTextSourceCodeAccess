 /* Copyright (c) 2014 K. Ernest 'iFire' Lee

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include "SublimeTextSourceCodeAccessor.h"
#include "SublimeTextSourceCodeAccessPrivatePCH.h"
#include "DesktopPlatformModule.h"

#define LOCTEXT_NAMESPACE "SublimeTextSourceCodeAccessor"

bool FSublimeTextSourceCodeAccessor::CanAccessSourceCode() const
{
    return FPaths::FileExists(TEXT("/usr/bin/clang"));
}

bool FSublimeTextSourceCodeAccessor::DoesSolutionExist() const
{
    FString FullPath;
    if ( FDesktopPlatformModule::Get()->GetSolutionPath(FullPath) )
    {
        return FPaths::FileExists(FullPath);
    }
    return false;
}

bool FSublimeTextSourceCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath)
{
    FString FullPath = InSolutionPath;
    if ( FPaths::FileExists(FullPath) )
    {
        // Add this to handle spaces in path names.
        const FString NewFullPath = FString::Printf(TEXT("\"%s\""), *FullPath);

        FString Editor = FString(TEXT("/usr/bin/subl"));
        if ( FLinuxPlatformProcess::CreateProc(
                *Editor,
                *NewFullPath,
                true,
                true,
                false,
                nullptr,
                0,
                nullptr,
                nullptr
            ).IsValid() )
        {
            return true;
        }
    }
    return false;
}


FName FSublimeTextSourceCodeAccessor::GetFName() const
{
    return FName("SublimeTextSourceCodeAccessor");
}

FText FSublimeTextSourceCodeAccessor::GetNameText() const
{
    return LOCTEXT("SublimeTextDisplayName", "Sublime Text");
}

FText FSublimeTextSourceCodeAccessor::GetDescriptionText() const
{
    return LOCTEXT("SublimeTextDisplayDesc", "Open source code files with Sublime Text");
}

bool FSublimeTextSourceCodeAccessor::OpenSolution()
{
    FString FullPath;
    if ( FDesktopPlatformModule::Get()->GetSolutionPath(FullPath) )
    {
        if ( FPaths::FileExists(FullPath) )
        {
            // Add this to handle spaces in path names.
            const FString NewFullPath = FString::Printf(TEXT("\"%s\""), *FullPath);

            FString Editor = FString(TEXT("/usr/bin/subl"));
            if ( FLinuxPlatformProcess::CreateProc(
                    *Editor,
                    *NewFullPath,
                    true,
                    true,
                    false,
                    nullptr,
                    0,
                    nullptr,
                    nullptr
                ).IsValid() )
            {
                return true;
            }
        }
    }

    return false;
}

bool FSublimeTextSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
    FString Editor = FString(TEXT("/usr/bin/subl"));

    // Add this to handle spaces in path names.
    const FString NewFullPath = FString::Printf(TEXT("\"%s:%d:%d\""), *FullPath, LineNumber, ColumnNumber);

    if ( FLinuxPlatformProcess::CreateProc(
            *Editor,
            *NewFullPath,
            true,
            true,
            false,
            nullptr,
            0,
            nullptr,
            nullptr
        ).IsValid() )
    {
        return true;
    }

    return false;
}

bool FSublimeTextSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
    for ( const FString& SourcePath : AbsoluteSourcePaths )
    {
        FString Editor = FString(TEXT("/usr/bin/subl"));

        // Add this to handle spaces in path names.
        const FString NewSourcePath = FString::Printf(TEXT("\"%s\""), *SourcePath);

        if ( !(FLinuxPlatformProcess::CreateProc(
                *Editor,
                *NewSourcePath,
                true,
                true,
                false,
                nullptr,
                0,
                nullptr,
                nullptr
            ).IsValid()) )
        {
            return false;
        }
    }

    return true;
}

bool FSublimeTextSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
    return false;
}

bool FSublimeTextSourceCodeAccessor::SaveAllOpenDocuments() const
{
    return false;
}

void FSublimeTextSourceCodeAccessor::Tick(const float DeltaTime)
{

}

#undef LOCTEXT_NAMESPACE
