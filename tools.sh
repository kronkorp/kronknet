DEBUG_DIRECTORY=Debug
RELEASE_DIRECTORY=Release
SOURCE_DIRECTORY=./

fclean() {
    echo "+=-----------------------=+"
    echo "| Cleaning all project... |"
    echo "+=-----------------------=+"
    rm -rf $DEBUG_DIRECTORY $RELEASE_DIRECTORY zappy_* lib/ compile_commands.json
}

###############################

release() {
    echo "+=---------------------------=+"
    echo "| Building Release project... |"
    echo "+=---------------------------=+"

    mkdir -p $RELEASE_DIRECTORY
    cmake   -S $SOURCE_DIRECTORY  \
            -B $RELEASE_DIRECTORY   \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        && cmake --build $RELEASE_DIRECTORY/ -j
    rm -rf compile_commands.json
    ln -s $RELEASE_DIRECTORY/compile_commands.json ./compile_commands.json
}

clean_release() {
    echo "+=---------------------------=+"
    echo "| Cleaning Release project... |"
    echo "+=---------------------------=+"
    rm -rf $RELEASE_DIRECTORY
}

#################################

debug() {
    echo "+=-------------------------=+"
    echo "| Building Debug project... |"
    echo "+=-------------------------=+"

    mkdir -p $DEBUG_DIRECTORY
    cmake   -S $SOURCE_DIRECTORY  \
            -B $DEBUG_DIRECTORY   \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        && cmake --build $DEBUG_DIRECTORY/ -j
    rm -rf compile_commands.json
    ln -s $DEBUG_DIRECTORY/compile_commands.json ./compile_commands.json
}

clean_debug() {
    echo "+=-------------------------=+"
    echo "| Cleaning Debug project... |"
    echo "+=-------------------------=+"
    rm -rf $DEBUG_DIRECTORY
}

################################

help() {
    echo "Usage: $0 [OPTIONS...]"
    echo "Options:"
    echo "  -r,   --release    Build the project in Release mode."
    echo "  -rer, --rerelease  Clean and build the project in Release mode."
    echo "  -d,   --debug      Build the project in Debug mode."
    echo "  -red, --redebug    Clean and build the project in Debug mode."
    echo "  -fc,  --fclean     Clean all build files."
}

if [ $# -eq 0 ]; then
    help
    exit 1
fi

while true; do
  if [ $# -eq 0 ]; then
    exit 0
  fi
  case "$1" in
    -r   | --release )   release;                  ;;
    -rer | --rerelease ) clean_release; release;   ;;
    -d   | --debug )     debug;                    ;;
    -red | --redebug )   clean_debug; debug;       ;;
    -fc  | --fclean)     fclean;                   ;;
    -h   | --help  )     help;                     ;;
    *  )                 help;                     exit 1 ;;
  esac
  shift
done