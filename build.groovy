#!/usr/bin/groovy
import groovy.transform.Field

import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths
import java.util.function.Consumer

@Field def CC = 'i686-elf-g++'
@Field def AS = 'nasm'

@Field def CFLAGS = '-ffreestanding -fno-exceptions -Wall -Wextra'
@Field def INCLUDE = '-I../mish/include" -I../feta/include -Iinclude'

@Field def TTY = 'tty'.execute().text
println TTY
if (args) {
	"${args.head()}"(*args.tail())
}

def compile(String folder = 'src') {
	compile(Paths.get(folder))
}

def compile(Path toCompile) {
	if (Files.isDirectory(toCompile)) {
		Files.list(toCompile).forEach { compile it }
	} else if (toCompile =~ /.cpp/) {
		String srcFile = toCompile
		String objectFile = toCompile.fileName.toString().replace('src', 'build').replace('cpp', 'o')
		
		execute "$CC $srcFile -o $objectFile $INCLUDE"
	}
}

def execute(String command){
	"$command >> $TTY".execute().waitFor()
}