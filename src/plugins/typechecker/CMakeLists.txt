include (LibAddHaskellPlugin)

find_package (fa)

if (LIBFA_FOUND)

	find_package (Haskell)

	set (TYPECHECKER_GHC_PLUGIN_PATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/elektra/typechecker-ghc-plugin")
	set (TYPECHECKER_GHC_PLUGIN "${TYPECHECKER_GHC_PLUGIN_PATH}/${GHC_TARGET_PLATFORM}-ghc-${GHC_VERSION}-packages.conf.d/")

	configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Elektra/Typechecker.hs.in" "${CMAKE_CURRENT_BINARY_DIR}/Elektra/Typechecker.hs" @ONLY)

	add_haskell_plugin (typechecker
			    SANDBOX_ADD_SOURCES
			    "src/libs/typesystem/libfa"
			    "src/libs/typesystem/specelektra"
			    "src/libs/typesystem/spectranslator"
			    TEST_README
			    INSTALL_TEST_DATA)

	# if haskell was not found, the target will not be created and a message was already printed
	# so we can skip the extra installation procedures
	if (TARGET typechecker)

		if (DEPENDENCY_PHASE)
			configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/sandbox/sandbox.cabal.in"
					"${CMAKE_CURRENT_BINARY_DIR}/sandbox/sandbox.cabal"
					@ONLY) # no extra dependency required so we can inline it without needing a user sandbox
			add_custom_command (OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sandbox/cabal.sandbox.config"
					    COMMAND ${CABAL_EXECUTABLE} sandbox delete -v0
					    COMMAND ${CABAL_EXECUTABLE} sandbox init -v0
					    COMMAND ${CABAL_EXECUTABLE} sandbox add-source -v0
						    "${CMAKE_BINARY_DIR}/src/libs/typesystem/libfa"
					    COMMAND ${CABAL_EXECUTABLE} sandbox add-source -v0
						    "${CMAKE_BINARY_DIR}/src/libs/typesystem/specelektra"
					    COMMAND ${CABAL_EXECUTABLE} install --only-dependencies --avoid-reinstalls --offline -v0
					    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sandbox/")
			add_custom_target ("typechecker-ghc-plugin"
					   ALL
					   DEPENDS c2hs_haskell
						   "${CMAKE_CURRENT_BINARY_DIR}/sandbox/cabal.sandbox.config")
		endif (DEPENDENCY_PHASE)

		# copy our small typechecker sandbox to the library dir so it can be easily deployed and use the dependencies
		# for hint from there
		install (DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sandbox/.cabal-sandbox/" DESTINATION ${TYPECHECKER_GHC_PLUGIN_PATH})
		# copy our prelude library so it can be always used for mounting typechecker specifications
		install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/typechecker/prelude.ini" DESTINATION ${KDB_DB_SPEC})

	endif (TARGET typechecker)

	mark_as_advanced (TYPECHECKER_GHC_PLUGIN TYPECHECKER_GHC_PLUGIN_PATH)

else (LIBFA_FOUND)
	remove_plugin (typechecker "libfa not found (usually bundled with augeas), but required for the typechecker plugin")
endif (LIBFA_FOUND)
