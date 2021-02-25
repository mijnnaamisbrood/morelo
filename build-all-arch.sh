#!/bin/bash

 set -e

 orig_path=$PATH
 base_dir=`pwd`

 build_type=release # or debug

 android_api=21
 archs=(arm arm64 x86 x86_64)
 #archs=(arm64)

 orig_cxx_flags=$CXXFLAGS
 for arch in ${archs[@]}; do
    extra_cmake_flags=""

    case ${arch} in
      "arm")
			   target_host=arm-linux-androideabi
			   xarch="armv7-a"
			   sixtyfour=OFF
			   extra_cmake_flags="-D NO_AES=true"
			   ;;
      "arm64")
			   target_host=aarch64-linux-android
			   xarch="armv8-a"
			   sixtyfour=ON
			   ;;
      "x86")
			   target_host=i686-linux-android
			   xarch="i686"
                           sixtyfour=OFF
			   ;;
      "x86_64")
			   target_host=x86_64-linux-android
			   xarch="x86-64"
			   sixtyfour=ON
			   ;;
      *)
			   exit 16
                           ;;
    esac

    OUTPUT_DIR=$base_dir/build/$build_type.$arch
    rm -rf $OUTPUT_DIR
    mkdir -p $OUTPUT_DIR
    cd $OUTPUT_DIR

    export PKG_CONFIG_PATH="/opt/android/build/libsodium/$arch/lib/pkgconfig:/opt/android/build/libzmq/$arch/lib/pkgconfig"
    export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/opt/android/build/boost/$arch/include:/opt/android/build/cppzmq/include:/opt/android/build/libzmq/$arch/include

    if [ "$arch" == "arm" ]
    then
        PATH=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin:/opt/android/ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin:$orig_path
        C_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$android_api-clang
        CXX_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$android_api-clang++
    elif [ "$arch" == "arm64" ]
    then
        PATH=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin:/opt/android/ndk/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin:$orig_path
        C_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$android_api-clang
        CXX_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$android_api-clang++
    elif [ "$arch" == "x86" ]
    then
        PATH=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin:/opt/android/ndk/toolchains/x86-4.9/prebuilt/linux-x86_64/bin:$orig_path
        C_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$android_api-clang
        CXX_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$android_api-clang++
    elif [ "$arch" == "x86_64" ]
    then
        PATH=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin:/opt/android/ndk/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin:$orig_path
        C_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$android_api-clang
        CXX_COMP=/opt/android/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$android_api-clang++
    fi

    CC=clang CXX=clang++ \
    cmake \
      -D BUILD_GUI_DEPS=1 \
      -D CMAKE_C_COMPILER=$C_COMP \
      -D CMAKE_CXX_COMPILER=$CXX_COMP \
      -D BUILD_TESTS=OFF \
      -D ARCH="$xarch" \
      -D STATIC=ON \
      -D BUILD_64=$sixtyfour \
      -D CMAKE_BUILD_TYPE=$build_type \
      -D CMAKE_CXX_FLAGS="-D__ANDROID_API__=$android_api -isystem /opt/android/build/libsodium/$arch/include/" \
      -D ANDROID=true \
      -D BUILD_TAG="android" \
      -D BOOST_ROOT=/opt/android/build/boost/$arch \
      -D OPENSSL_INCLUDE_DIR=/opt/android/build/include \
      -D OPENSSL_CRYPTO_LIBRARY=/opt/android/build/openssl/$arch/lib/libcrypto.so \
      -D OPENSSL_SSL_LIBRARY=/opt/android/build/openssl/$arch/lib/libssl.so \
      -D CMAKE_POSITION_INDEPENDENT_CODE:BOOL=true \
       $extra_cmake_flags ../.. -B $OUTPUT_DIR

    make -j$(nproc) wallet_api VERBOSE=1

    TARGET_LIB_DIR=/opt/android/build/arqma/$arch/lib
    rm -rf $TARGET_LIB_DIR
    mkdir -p $TARGET_LIB_DIR
    find $OUTPUT_DIR -name '*.a' -exec cp '{}' $TARGET_LIB_DIR \;

    TARGET_INC_DIR=/opt/android/build/arqma/include
    rm -rf $TARGET_INC_DIR
    mkdir -p $TARGET_INC_DIR
    cp -a ../../src/wallet/api/wallet2_api.h $TARGET_INC_DIR

    cd $base_dir
done
exit 0