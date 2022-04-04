package main

import (
	"encoding/json"
	"fmt"
	"os/exec"
	"net/http"
	"os"
  "time"
)

import (
	"github.com/gorilla/mux"
)

var PIN=os.Getenv("PIN")
//read_only
var play_status = "playing"
var version = "Blyat-Blaster-9000.3.4.1.5.9"
var play_info_name = "Ще не вмерла України і слава, і воля"
var play_info_text = "(Найкращі хіти)"

func welcome(w http.ResponseWriter, r *http.Request) {
  w.Write([]byte("<!DOCTYPE html><img width=\"1024\" src=\"sinot_aqua.webp\"/><body>Congratulations on your new SuperYacht. Please refer to your Blyat Blaster Stereo System 9000 manual for setup and usage instructions.</body></html>"))
}

func img(w http.ResponseWriter, r *http.Request) {
	blyat, _ := os.ReadFile("sinot_aqua.webp")
  w.Write([]byte(blyat))
}


func read_only(w http.ResponseWriter, r *http.Request) {
	pin := r.URL.Query().Get("code")

	if (pin != PIN) {
		http.Error(w, "Wrong pin code", 401)
		return
	}

	v := map[string]string{
		"play_status" : play_status,
		"version": version,
		"play_info_name": play_info_name,
		"play_info_text": play_info_text,

	}

	ret := v[r.URL.Path[1:]]
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(ret)
}

var volume = 50
var friendly_name = "Eclipse Blyat Boat"
var mute = false
var power = 20

func read_write(w http.ResponseWriter, r *http.Request) {
	pin := r.URL.Query().Get("code")

	if (pin != PIN) {
		http.Error(w, "Wrong pin code", 401)
		return
	}

	target := r.URL.Path[1:]
	do_write := false

	w.Header().Set("Content-Type", "application/json")

	if r.Method == http.MethodPut || r.Method == http.MethodPost {
		do_write = true
	}


	switch target {
	case "volume":
		if do_write {
			json.NewDecoder(r.Body).Decode(&volume)
		}
		json.NewEncoder(w).Encode(volume)
		break
	case "friendly_name":
		not_friendly_name := friendly_name
		if do_write {
			//dont write to actual friendly to avoid leaking answer
			not_friendly_name := ""
			err := json.NewDecoder(r.Body).Decode(&not_friendly_name)
			if (err != nil) {
				http.Error(w, err.Error(), 400)
				return
			}
			if len(not_friendly_name) > 30 {
				not_friendly_name = not_friendly_name[:30]
			}
			update_wifi_name(not_friendly_name)
		}
		json.NewEncoder(w).Encode(not_friendly_name)
		break
	case "power":
		if do_write {
			json.NewDecoder(r.Body).Decode(&power)
		}
		json.NewEncoder(w).Encode(power)
		break
	case "mute":
		if do_write {
			json.NewDecoder(r.Body).Decode(&mute)
		}
		json.NewEncoder(w).Encode(mute)
		break
	}

}

func update_wifi_name(friendly_name string) {
	cmd := exec.Command("/bin/sh","-c", "hostapd -p /tmp/control set ssid " + friendly_name)
	stdout, err := cmd.Output()
	if err != nil {
		fmt.Println("command fail", err)
	}
	fmt.Println(string(stdout))
}


func logRequest(handler http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("%s %s %s\n", r.RemoteAddr, r.Method, r.URL)
		handler.ServeHTTP(w, r)
	})
}

func main() {
	fmt.Println("[+] Started webserver " + PIN)

	if (PIN == "")  {
		PIN = "1337"
	}

	router := mux.NewRouter()


	router.HandleFunc("/welcome.html", welcome).Methods("GET")
	router.HandleFunc("/sinot_aqua.webp", img).Methods("GET")

	router.HandleFunc("/play_status", read_only).Methods("GET")
	router.HandleFunc("/version", read_only).Methods("GET")
	router.HandleFunc("/play_info_name", read_only).Methods("GET")
	router.HandleFunc("/play_info_text", read_only).Methods("GET")

	router.HandleFunc("/volume", read_write).Methods("GET","PUT", "POST")
	router.HandleFunc("/friendly_name", read_write).Methods("GET","PUT", "POST")
	router.HandleFunc("/power", read_write).Methods("GET","PUT", "POST")
	router.HandleFunc("/mute", read_write).Methods("GET","PUT", "POST")

	http.ListenAndServe("0.0.0.0:8080", http.TimeoutHandler(logRequest(router), time.Second*3, "Timeout!"))
}
