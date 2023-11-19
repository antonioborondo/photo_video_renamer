install(TARGETS photo_video_renamer DESTINATION bin)

if(WIN32)
    install(FILES res/icon.ico DESTINATION res)

    set(VC_REDISTRIBUTABLE ${CMAKE_BINARY_DIR}/vc_redist.x64.exe)

    if(NOT EXISTS ${VC_REDISTRIBUTABLE})
        message(STATUS "Downloading Microsoft Visual C++ Redistributable from https://aka.ms/vs/17/release/vc_redist.x64.exe")
        file(DOWNLOAD "https://aka.ms/vs/17/release/vc_redist.x64.exe" ${VC_REDISTRIBUTABLE})
    endif()

    install(PROGRAMS ${VC_REDISTRIBUTABLE} DESTINATION bin)

    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})

    set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
        "
        ExecWait '$INSTDIR\\\\bin\\\\vc_redist.x64.exe /install /quiet /norestart'
        WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer' '' 'Rename photos and videos'
        WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer' 'Icon' '$INSTDIR\\\\res\\\\icon.ico'
        WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer\\\\command' '' '$INSTDIR\\\\bin\\\\photo_video_renamer.exe %1'
        WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer' '' 'Rename photos and videos'
        WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer' 'Icon' '$INSTDIR\\\\res\\\\icon.ico'
        WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer\\\\command' '' '$INSTDIR\\\\bin\\\\photo_video_renamer.exe'
        EnVar::SetHKLM
        EnVar::AddValue 'PATH' '$INSTDIR\\\\bin'
        "
    )

    SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
        "
        DeleteRegKey HKCR 'Directory\\\\shell\\\\photo_video_renamer'
        DeleteRegKey HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer'
        EnVar::SetHKLM
        EnVar::DeleteValue 'PATH' '$INSTDIR\\\\bin'
        "
    )
endif()

if(NOT WIN32)
    set(CPACK_PACKAGE_CONTACT "Antonio Borondo <antonioborondo@outlook.com>")
endif()

include(CPack)
