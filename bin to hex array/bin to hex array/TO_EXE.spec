# -*- mode: python -*-

block_cipher = None


a = Analysis(['bin_to_hex_array.py'],
             pathex=['C:\\Users\\jcliu\\Documents\\Visual Studio 2013\\Projects\\bin to hex array\\bin to hex array'],
             binaries=[],
             datas=[],
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          exclude_binaries=True,
          name="BIN_TO_ARRAY",
          debug=False,
          strip=False,
		  onefile =True,
          upx=True,
          console=True )
coll = COLLECT(exe,
               a.binaries,
               a.zipfiles,
               a.datas,
               strip=False,
               upx=True,
			   onefile =True,
               name='BIN_TO_ARRAY')

