//
//  File: DKDirectory.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2017 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKFile.h"
#include "DKString.h"
#include "DKArray.h"

namespace DKFoundation
{
	/**
	 @brief provide file-system directory access.
	 @note
	  - OpenDir(const DKString&) argument must be absolute-path.
	  - real time monitoring not supported.

	 @note
	  - On win32
		- root('/') is virtual path. that is parent path for all drive letters.
		  - (ie, '/C:\' means 'C:\')
		- '/' is always read-only in Win32.
		- You can ignore '/' character for root path. (using 'C:', 'C:\', is ok)

	 @verbatim

	  on Mac OS X
	  '/' (root, mounted as root)
	   +---- 'Applications'
	   |      +--- 'Any Applications...'
	   +---- 'Users'
	   |      +--- 'Users Home Dirs...'
	   +---- 'Library'
	   ...

	 on Windows
	  '/' (root, virtual-path)
	   +---- 'C:'
	   |      +--- 'Program Files'
	   |      +--- 'Windows'
	   |      \--- 'Any Sub Dirs in C:...'
	   +---- 'D:'
	   |      \--- 'Some Dirs in D:...'
	   +---- 'E:'
	   \---- 'F:'

	 @endverbatim

	 @note
	  On windows
	   - universal naming convention (UNC) not supported!
	   - long path prefix (\\\\?\\) not supported.
	 */
	class DKGL_API DKDirectory
	{
	public:
		DKDirectory();
		~DKDirectory();

		static DKObject<DKDirectory> OpenDir(const DKString& path);
		static bool IsDirExist(const DKString& path); ///< should be absolute-path

		bool operator == (const DKDirectory& dir) const;
		bool operator != (const DKDirectory& dir) const;

		/// retrieve current path, absolute-path
		const DKString& AbsolutePath() const;

		/// return absolute-path if file exists, or return empty string otherwise.
		DKString AbsolutePathOfFile(const DKString& name) const;
		/// return absolute-path if sub-dir exists, or return empty string otherwise.
		DKString AbsolutePathOfSubdir(const DKString& name) const;

		/// open file by file-name. (which located in directory)
		DKObject<DKFile> OpenFile(const DKString& name, DKFile::ModeOpen mode, DKFile::ModeShare share) const;
		/// open file by file-index
		DKObject<DKFile> OpenFile(unsigned int index, DKFile::ModeOpen mode, DKFile::ModeShare share) const;

		/// map file as memory by file-name (file-map, using mmap)
		DKObject<DKData> MapFile(const DKString& name, size_t size, bool writable) const;
		/// map file as memory by file-index (file-map, using mmap)
		DKObject<DKData> MapFile(unsigned int index, size_t size, bool writable) const;

		/// open sub-directory
		DKObject<DKDirectory> OpenSubdir(const DKString& name) const;
		DKObject<DKDirectory> OpenSubdirAtIndex(unsigned int index) const;
		/// open parent-directory
		DKObject<DKDirectory> OpenParent() const;
		/// create new sub-directory
		DKObject<DKDirectory> CreateDir(const DKString& name) const;

		bool IsFileExist(const DKString& name) const;
		bool IsSubdirExist(const DKString& name) const;
		bool IsValid() const;	///< test directory validation whether or not
		bool IsReadable() const;
		bool IsWritable() const;

		bool Reload() const; ///< reload contents

		size_t NumberOfFiles() const;
		size_t NumberOfSubdirs() const;
		const DKString& FileNameAtIndex(unsigned int index) const;
		const DKString& SubdirNameAtIndex(unsigned int index) const;

		bool DeleteSubDir(const DKString& name) const;

	private:
		typedef DKArray<DKString> StringList;
		void ReloadIfNeeded() const;

		DKString currentPath;   // absolute path of current object.
		StringList files;       // directory contents (files)
		StringList directories; // directory contents (sub-dirs)
		bool reloadRequired;
	};	
}

