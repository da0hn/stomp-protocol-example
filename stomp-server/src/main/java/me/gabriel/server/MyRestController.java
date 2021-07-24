package me.gabriel.server;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author daohn
 * @since 24/07/2021
 */
@RestController
public class MyRestController {

  private final SimpMessagingTemplate template;

  @Autowired
  public MyRestController(SimpMessagingTemplate template) {
    this.template = template;
  }

  @GetMapping("/enviar")
  public void send() {
    System.out.println("Enviando mensagem");
    this.template.convertAndSend("/channel/output-1", "Olá esp");

  }

}
