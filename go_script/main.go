package main

import (
	"bufio"
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
	"time"
)

const (
	mainURL = "http://www.scs.stanford.edu/10wi-cs140/pintos/specs/kbd/scancodes-9.html"
)

var (
	flag  bool
	count = 0

	index1 = 0
)

type CsvLine struct {
	Column1 string
	Column2 string
}

type KeysAndHex struct {
	header1 string
	header2 string
	header3 string
	header4 string
	header5 string
	header6 string
	header7 string
	header8 string
}

func main() {
	table := [111][10]string{}
	csv_file, _ := os.Open("abc.csv")

	scanner := bufio.NewScanner(csv_file)
	for scanner.Scan() {
		lineSlice := strings.Split(scanner.Text(), ",")
		for i, v := range lineSlice {
			table[index1][i] = v

		}
		index1++
	}
	// fmt.Println(FixSpecialChar(table[1][8]))
	// // Print the info in the ScanCode.h
	// for _, v := range table {
	// 	// fmt.Println(v)
	// 	fmt.Printf("KEY_%s = 0x%s\n", FixSpecialChar(v[8]), strings.TrimSpace(v[1]))
	// 	time.Sleep(1 * time.Second)
	// }

	PrintLine(table)
}

func extractInformation() {
	// Browse the URL and download the webpage
	resp, err := http.Get(mainURL)
	if err != nil {
		log.Println("Couldn't reach!")
		os.Exit(1)
	}
	defer resp.Body.Close()

	// Create a CSV File
	// _, err = os.Create("abc.csv")
	// if err != nil {
	// 	log.Println("Error occoured while creating the csv file")
	// }

	// // Open file
	// csvFile, err := os.OpenFile("abc.csv", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	// if err != nil {
	// 	log.Fatal(err)
	// }
	// defer csvFile.Close()

	// Scan the webpage and store the value in CSV file
	scanner := bufio.NewScanner(resp.Body)
	for scanner.Scan() {
		if strings.Contains(scanner.Text(), "</TD><TD>") {
			if count > 16 {
				// fmt.Println(strings.Replace(scanner.Text(), "</TD><TD>", ",", 0))

				line := scanner.Text()
				dfLine := strings.ReplaceAll(line, "</TD><TD>", ",")
				dfLine = strings.ReplaceAll(dfLine, "</TD></TR><TR><TD>", "")
				fmt.Println(dfLine)
				// csvFile.WriteString(dfLine)
				time.Sleep(1)
			}
			count++
		}
	}
}

func FixSpecialChar(s string) string {
	if strings.TrimSpace(s) == "` ~" {
		return "Tilde"
	} else if strings.TrimSpace(s) == "[ {" {
		return "LBRACKET"
	} else if strings.TrimSpace(s) == "] }" {
		return "RBRACKET"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "; :" {
		return "SEMICOLON"
	} else if strings.TrimSpace(s) == "' \"" {
		return "APOSTROPHE"
	} else if strings.TrimSpace(s) == ", &lt" {
		return "COMMA"
	} else if strings.TrimSpace(s) == ". &gt" {
		return "PERIOD"
	} else if strings.TrimSpace(s) == "- _" {
		return "DASH"
	} else if strings.TrimSpace(s) == "= +" {
		return "PLUS"
	} else if strings.TrimSpace(s) == "/ ?" {
		return "SLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
		return "BACKSLASH"
	} else if strings.TrimSpace(s) == "\\ |" {
	}
	// return strings.ContainsAny()r
	return strings.TrimSpace(s)
}

func PrintLine(table [111][10]string) {

	for idx, h := range table[0] {
		if idx != 0 && idx < 9 {
			fmt.Println("typedef enum {")
			for i, v := range table {
				if i != 0 {
					fmt.Printf("\tKEY_%s = 0x%s\n", FixSpecialChar(v[8]), strings.TrimSpace(v[1]))
					time.Sleep(1 * time.Millisecond)
				}
			}
			fmt.Printf("} %s;\n\n", CleanHeader(h))
		}
	}

}

func CleanHeader(h string) string {
	if strings.ContainsAny(h, "( )") {

		return "Implement REGEXP"
	}
	return ""
}
